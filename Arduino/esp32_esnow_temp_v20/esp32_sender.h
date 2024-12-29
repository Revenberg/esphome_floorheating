/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-auto-pairing-esp32-esp8266/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. 
  Based on JC Servaye example: https://github.com/Servayejc/esp_now_sender/
*/
#include <Arduino.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <ArduinoJson.h>

static bool DEBUG_SENDER = true;
long send_interval = 30000;
unsigned long ResetEventTime = 100000;

static unsigned long lastEventTime = millis();

JsonDocument doc;
JsonObject devices;

// Set your Board and Server ID
uint64_t board_id;
#define MAX_CHANNEL 13  // 11 in North America or 13 in Europe

uint8_t serverAddress[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
uint8_t clientMacAddress[6];

//Structure to ESP32 component send data
enum esp32ComponentType { TEMPERATURE_COMPONENT,
                          HUMIDITY_COMPONENT,
                          SWITCH_COMPONENT,
};

// Structure to send data
// Must match the receiver structure
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  uint8_t msgType;
  uint64_t id;
  uint8_t macAddr[6];
  esp32ComponentType component_type;
  uint8_t component_id;
  float component_value;
  unsigned int readingId;
} struct_message;

typedef struct struct_ESP32_Component_Type {
  esp32ComponentType component_type;
  uint8_t component_count;
} struct_ESP32_Component_Type;

typedef struct struct_pairing {  // new structure for pairing
  uint8_t msgType;
  uint64_t id;
  uint8_t macAddr[6];
  uint8_t channel;
  uint8_t esp32ComponentCount;
  struct_ESP32_Component_Type esp32ComponentArray[20];
} struct_pairing;

enum ESP_NOW_COMMANDS { RESET,
                        INTERVAL,
                        FABRIC };

typedef struct struct_command {  // new structure for pairing
  uint8_t msgType;
  uint64_t id;
  uint8_t macAddr[6];
  uint8_t channel;
  ESP_NOW_COMMANDS cmd;
  char parameters[200];
} struct_command;

esp_now_peer_info_t peer;

// Create 2 struct_message
struct_message myData;  // data to send
struct_message inData;  // data received
struct_pairing pairingData;
struct_command commandData;

enum PairingStatus { NOT_PAIRED,
                     PAIR_REQUEST,
                     PAIR_REQUESTED,
                     PAIR_PAIRED,
};
PairingStatus pairingStatus = NOT_PAIRED;

enum MessageType { PAIRING,
                   DATA,
                   CMD,
                   KEEPALIVE
};
MessageType messageType;

//#ifdef SAVE_CHANNEL
//  int lastChannel;
//#endif
int channel = 1;

unsigned long start;  // used to measure Pairing time
unsigned int readingId = 0;

unsigned long currentMillis = millis();
unsigned long previousMillis = 0;  // Stores last time temperature was published

String formatMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  return macStr;
}

uint64_t mac2int(String s) {
  uint64_t r = 0;

  for (int i = 0; i < 17; ++i) {
    char let = s[i];
    if (let >= '0' && let <= '9') {
      r = r * 0x10 + (let - '0');
    } else if (let >= 'a' && let <= 'f') {
      r = r * 0x10 + (let - 'a' + 10);
    } else if (let >= 'A' && let <= 'F') {
      r = r * 0x10 + (let - 'A' + 10);
    }
  }
  return r;
}

void readMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (DEBUG_SENDER) {
    if (ret == ESP_OK) {
      Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                    baseMac[0], baseMac[1], baseMac[2],
                    baseMac[3], baseMac[4], baseMac[5]);
    } else {
      Serial.println("Failed to read MAC address");
    }
  }
  clientMacAddress[0] = baseMac[0];
  clientMacAddress[1] = baseMac[1];
  clientMacAddress[2] = baseMac[2];
  clientMacAddress[3] = baseMac[3];
  clientMacAddress[4] = baseMac[4];
  clientMacAddress[5] = baseMac[5];
  board_id = mac2int(formatMAC(clientMacAddress));
}

void addPeer(const uint8_t *mac_addr, uint8_t chan) {
  ESP_ERROR_CHECK(esp_wifi_set_channel(chan, WIFI_SECOND_CHAN_NONE));
  esp_now_del_peer(mac_addr);
  memset(&peer, 0, sizeof(esp_now_peer_info_t));
  peer.channel = chan;
  peer.encrypt = false;
  memcpy(peer.peer_addr, mac_addr, sizeof(uint8_t[6]));
  if (esp_now_add_peer(&peer) != ESP_OK) {
    if (DEBUG_SENDER) {
      Serial.println("Failed to add peer");
      return;
    }
  }
  memcpy(serverAddress, mac_addr, sizeof(uint8_t[6]));
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (DEBUG_SENDER) {
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
}

void executeESPCommand(ESP_NOW_COMMANDS cmd, char parameters[200]) {
  switch (cmd) {
    case RESET:
      ESP.restart();
      break;
    case INTERVAL:
      send_interval = atoi(parameters);
      storeConfig(doc);
      break;
    case FABRIC:
      removeConfig();
      ESP.restart();
      break;
  }
}

void printMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  uint8_t type = incomingData[0];
  if (DEBUG_SENDER) {
    Serial.print("Packet received with data size = ");
    Serial.println(sizeof(incomingData));
    Serial.println("type = type");
  }
  switch (type) {
    case KEEPALIVE:
      lastEventTime = millis();
      break;
    case DATA:  // we received data from server
      memcpy(&inData, incomingData, sizeof(inData));
      if (DEBUG_SENDER) {
        Serial.print("Size of message : ");
        Serial.print(len);
        Serial.print(" from ");
        printMAC(mac_addr);
        Serial.println();
      }
      break;
    case CMD:
      if (inData.id == board_id) {
        memcpy(&commandData, incomingData, sizeof(commandData));
        if (commandData.id == 0) {
          executeESPCommand(commandData.cmd, commandData.parameters);
        }
        if (commandData.id == board_id) {
          executeESPCommand(commandData.cmd, commandData.parameters);
        }
      }
      break;
    case PAIRING:  // we received pairing data from server
      memcpy(&pairingData, incomingData, sizeof(pairingData));
      if (pairingData.id == 0) {  // the message comes from server
        if (DEBUG_SENDER) {
          Serial.print("Pairing done for MAC Address: ");
          printMAC(pairingData.macAddr);
          Serial.print(" on channel ");
          Serial.print(pairingData.channel);  // channel used by the server
          Serial.print(" in ");
          Serial.print(millis() - start);
          Serial.println("ms");
        }
        addPeer(pairingData.macAddr, pairingData.channel);  // add the server  to the peer list
        pairingStatus = PAIR_PAIRED;                        // set the pairing status
        lastEventTime = millis();
      }
      break;
  }
}

PairingStatus autoPairing() {
  switch (pairingStatus) {
    case PAIR_REQUEST:      
      if (DEBUG_SENDER) {
        Serial.print("Pairing request on channel ");
        Serial.println(channel);
      }

      // set WiFi channel
      ESP_ERROR_CHECK(esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE));
      if (esp_now_init() != ESP_OK) {
        if (DEBUG_SENDER) {
          Serial.println("Error initializing ESP-NOW");
        }
      }
      // set callback routines
      esp_now_register_send_cb(OnDataSent);
      esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

      // add peer and send request
      addPeer(serverAddress, channel);
      esp_now_send(serverAddress, (uint8_t *)&pairingData, sizeof(pairingData));
      previousMillis = millis();
      pairingStatus = PAIR_REQUESTED;
      break;

    case PAIR_REQUESTED:
      // time out to allow receiving response from server
      currentMillis = millis();
      if (currentMillis - previousMillis > 1000) {
        previousMillis = currentMillis;
        // time out expired,  try next channel
        channel++;
        if (channel > MAX_CHANNEL) {
          channel = 1;
        }
        pairingStatus = PAIR_REQUEST;
      }
      break;

    case PAIR_PAIRED:      
      break;
  }
  return pairingStatus;
}

void esp_now_setup(int numberOfDevices, esp32ComponentType esp32Component) {
  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();

  if (DEBUG_SENDER) {
    Serial.begin(115200);
    Serial.print("Client Board MAC Address:  ");
  }
  readMacAddress();
  WiFi.disconnect();
  start = millis();

  // set pairing data to send to the server
  pairingData.msgType = PAIRING;
  pairingData.id = board_id;
  pairingData.channel = channel;
  pairingData.macAddr[0] = clientMacAddress[0];
  pairingData.macAddr[1] = clientMacAddress[1];
  pairingData.macAddr[2] = clientMacAddress[2];
  pairingData.macAddr[3] = clientMacAddress[3];
  pairingData.macAddr[4] = clientMacAddress[4];
  pairingData.macAddr[5] = clientMacAddress[5];

  pairingData.esp32ComponentArray[0].component_type = esp32Component;
  pairingData.esp32ComponentArray[0].component_count = numberOfDevices;
  pairingData.esp32ComponentCount = 1;
  pairingStatus = PAIR_REQUEST;
}

void esp_now_20_send(struct_message myData) {
  myData.id = board_id;
  myData.readingId = readingId++;

  myData.macAddr[0] = clientMacAddress[0];
  myData.macAddr[1] = clientMacAddress[1];
  myData.macAddr[2] = clientMacAddress[2];
  myData.macAddr[3] = clientMacAddress[3];
  myData.macAddr[4] = clientMacAddress[4];
  myData.macAddr[5] = clientMacAddress[5];

  esp_now_send(pairingData.macAddr, (uint8_t *)&myData, sizeof(myData));
}

void esp_loop() {  
  if (autoPairing() == PAIR_PAIRED) { 
    if ((millis() - lastEventTime) > ResetEventTime) {    
      Serial.print(millis());
      Serial.print(" - ");
      Serial.print(lastEventTime);
      Serial.print(" - ");
      Serial.print(ResetEventTime);
      Serial.print(" - ");
      Serial.println("pairingStatus = PAIR_REQUEST");
      pairingStatus = PAIR_REQUEST;
      lastEventTime = millis();
    }
  }
}