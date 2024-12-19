#include <ESP8266WiFi.h>
#include <espnow.h>

static bool DEBUG_SENDER = false;

unsigned long send_interval = 5000;
unsigned long ResetEventTime = 100000;

static unsigned long lastEventTime = millis();

unsigned long currentMillis = millis();
unsigned long timerDelay = 2000;  // send readings timer

unsigned long previousMillis = 0;  // will store last time DHT was updated

uint8_t channel = 1;
int readingId = 0;

int ledCounter = 0;
static unsigned long ledLastEventTime = millis();

#define MAX_CHANNEL 13  // 11 in North America or 13 in Europe

uint8_t broadcastAddressX[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

uint8_t clientMacAddress[6];

enum PairingStatus { PAIR_REQUEST,
                     PAIR_REQUESTED,
                     PAIR_PAIRED,
};
PairingStatus pairingStatus = PAIR_REQUEST;

enum MessageType { PAIRING,
                   DATA,
                   CMD,
                   KEEPALIVE };
//Structure to ESP32 component send data
enum esp32ComponentType { TEMPERATURE_COMPONENT,
                          HUMIDITY_COMPONENT,
                          SWITCH_COMPONENT,
};

typedef struct struct_ESP32_Component_Type {  // new structure for pairing
  esp32ComponentType component_type;
  uint8_t component_count;
} struct_ESP32_Component_Type;

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

static uint8_t ESP32COMPONENTCOUNT = 2;  // max 20

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
  uint8_t msgType;
  uint64_t id;
  uint8_t macAddr[6];
  esp32ComponentType component_type;
  uint8_t component_id;
  float component_value;
  unsigned int readingId;
} struct_message;

typedef struct struct_pairing {  // new structure for pairing
  uint8_t msgType;
  uint64_t id;
  uint8_t macAddr[6];
  uint8_t channel;
  uint8_t esp32ComponentCount;
  struct_ESP32_Component_Type esp32ComponentArray[20];
} struct_pairing;

// Create a struct_message called myData
struct_message myData;
struct_message incomingReadings;
struct_pairing pairingData;
struct_command commandData;

uint64_t board_id;
unsigned long start;

bool relay1_status = 0;
bool relay2_status = 0;

void relay1(bool value) {
  if (DEBUG_SENDER) Serial.println(" relay1");
  
  const byte close_relay[] = { 0xA0, 0x01, 0x01, 0xA2 };
  const byte open_relay[] = { 0xA0, 0x01, 0x00, 0xA1 };  
  (value) ? Serial.write(close_relay, sizeof(close_relay)) : Serial.write(open_relay, sizeof(open_relay));
  relay1_status = (value) ? 1 : 0;
}

void relay2(bool value) {
  if (DEBUG_SENDER) Serial.println("relay2");
  const byte close_relay[] = { 0xA0, 0x02, 0x01, 0xA3 };
  const byte open_relay[] = { 0xA0, 0x02, 0x00, 0xA2 };
  (value) ? Serial.write(close_relay, sizeof(close_relay)) : Serial.write(open_relay, sizeof(open_relay));
  relay2_status = (value) ? 1 : 0;
}

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  if (DEBUG_SENDER) {
    Serial.print("Last Packet Send Status: ");
    if (sendStatus == 0) {
      Serial.println("Delivery success");

    } else {
      Serial.println("Delivery fail");
    }
  }
}

void printMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
}

void executeESPCommand(ESP_NOW_COMMANDS cmd, char parameters[200]) {
  switch (cmd) {
    case RESET:
      ESP.restart();
      break;
    case INTERVAL:
      send_interval = atoi(parameters);
      break;
    case FABRIC:
      ESP.restart();
      break;
  }
}

// Callback when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  if (DEBUG_SENDER) {
    Serial.print("Size of message : ");
    Serial.print(len);
    Serial.print(" from ");
    printMAC(mac);
    Serial.println();
  }

  uint8_t type = incomingData[0];
  switch (type) {
    case KEEPALIVE:
      lastEventTime = millis();
      break;
    case DATA:
      memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
      if (DEBUG_SENDER) {
        Serial.print(len);
        Serial.print(" Data bytes received from: ");
        printMAC(mac);
        Serial.print(" incomingReadings.id: ");
        Serial.print(incomingReadings.id);
        Serial.print(" = board_id: ");
        Serial.print(board_id);
        Serial.println();
      }
      if (incomingReadings.id == board_id) {
        if (incomingReadings.component_type == SWITCH_COMPONENT) {
          if (incomingReadings.component_id == 1) {
            relay1(incomingReadings.component_value == 1);
          }
        }
        if (incomingReadings.component_type == SWITCH_COMPONENT) {
          if (incomingReadings.component_id == 2) {
            relay2(incomingReadings.component_value == 1);
          }
        }
      }
      break;
    case CMD:
      memcpy(&commandData, incomingData, sizeof(commandData));
      if (commandData.id == 0) {
        executeESPCommand(commandData.cmd, commandData.parameters);
      }
      if (commandData.id == board_id) {
        executeESPCommand(commandData.cmd, commandData.parameters);
      }
      break;
    case PAIRING:
      memcpy(&pairingData, incomingData, sizeof(pairingData));
      if (pairingData.id == 0) {  // the message comes from server
        if (DEBUG_SENDER) {
          Serial.print("Pairing done for ");
          printMAC(pairingData.macAddr);
          Serial.print(" on channel ");
          Serial.print(pairingData.channel);  // channel used by the server
          Serial.print(" in ");
          Serial.print(millis() - start);
          Serial.println("ms");
          Serial.print("board_id: ");
          Serial.println(board_id);
        }
        esp_now_del_peer(pairingData.macAddr);
        esp_now_del_peer(mac);
        esp_now_add_peer(pairingData.macAddr, ESP_NOW_ROLE_COMBO, pairingData.channel, NULL, 0);  // add the server to the peer list
        pairingStatus = PAIR_PAIRED;                                                              // set the pairing status
      }
      break;
  }
  ledCounter = 5;
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
  if (DEBUG_SENDER) {
    Serial.print("input: ");
    Serial.print(s);
    Serial.print(" result: ");
    Serial.println(r);
  }
  return r;
}

String formatMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  return macStr;
}

void readMacAddress() {
  String val = WiFi.macAddress();
  char *endPtr;

  clientMacAddress[0] = strtol(val.c_str(), &endPtr, 16);

  for (int i = 1; (*endPtr) && (i < 6); i++) {
    clientMacAddress[i] = strtol(endPtr + 1, &endPtr, 16);  // using +1 for the pointer as we want to skip the ':'
  }
  board_id = mac2int(formatMAC(clientMacAddress));
}

PairingStatus autoPairing() {
  switch (pairingStatus) {
    case PAIR_REQUEST:
      if (DEBUG_SENDER) {
        Serial.print("Pairing request on channel ");
        Serial.println(channel);
      }
      // clean esp now
      esp_now_deinit();
      WiFi.mode(WIFI_STA);
      readMacAddress();
      // set WiFi channel
      wifi_promiscuous_enable(1);
      wifi_set_channel(channel);
      wifi_promiscuous_enable(0);
      WiFi.disconnect();

      // Init ESP-NOW
      if (esp_now_init() != 0) {
        if (DEBUG_SENDER) {
          Serial.println("Error initializing ESP-NOW");
        }
      }
      esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
      // set callback routines
      esp_now_register_send_cb(OnDataSent);
      esp_now_register_recv_cb(OnDataRecv);

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

      pairingData.esp32ComponentCount = 1;
      pairingData.esp32ComponentArray[0].component_type = SWITCH_COMPONENT;
      pairingData.esp32ComponentArray[0].component_count = ESP32COMPONENTCOUNT;

      previousMillis = millis();
      // add peer and send request
      esp_now_send(broadcastAddressX, (uint8_t *)&pairingData, sizeof(pairingData));
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
          channel = 0;
        }
        pairingStatus = PAIR_REQUEST;
      }
      ledCounter = 5;
      break;

    case PAIR_PAIRED:
      break;
  }
  return pairingStatus;
}

void setup_esp8266() {
  // Init Serial Monitor
  pinMode(LED_BUILTIN, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  while (!Serial)
    ;
  readMacAddress();
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    if (DEBUG_SENDER) {
      Serial.println("Error initializing ESP-NOW");
    }
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);
}

void esp_loop() {
  if ((millis() - ledLastEventTime) > send_interval) {
    ledLastEventTime = millis();
    if (ledCounter > 0) {
      if (ledCounter % 2 == 1) {
        digitalWrite(LED_BUILTIN, LOW);        
      } else {
        digitalWrite(LED_BUILTIN, HIGH);        
      }
      ledCounter--;
    }
  }

  if (autoPairing() == PAIR_PAIRED) {
    if ((millis() - lastEventTime) > ResetEventTime) {
      pairingStatus = PAIR_REQUEST;
      lastEventTime = millis();
    }
  }
}