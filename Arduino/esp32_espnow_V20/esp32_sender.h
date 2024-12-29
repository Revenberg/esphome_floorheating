#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "AsyncTCP.h"
#include <ArduinoJson.h>

// Replace with your network credentials (STATION)
const char *ssid = "revenberg";
const char *password = "rev61272";

#define RXD2 16
#define TXD2 17
// Use Serial1 for UART communication
HardwareSerial mySerial(1);

esp_now_peer_info_t slave;
int chan;

enum MessageType { PAIRING,
                   DATA,
                   CMD,
                   KEEPALIVE
};
MessageType messageType;

int readingIdCounter = 0;

uint8_t clientMacAddress[6];
String ServerMacAddress;
String receivedMessage = "";

//AsyncEventSource events("/events");

typedef struct struct_ESP32_Component_Type {  // new structure for pairing
  esp32ComponentType component_type;
  uint8_t component_count;
} struct_ESP32_Component_Type;

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

typedef struct struct_pairing {  // new structure for pairing
  uint8_t msgType;
  uint64_t id;
  uint8_t macAddr[6];
  uint8_t channel;
  uint8_t esp32ComponentCount;
  struct_ESP32_Component_Type esp32ComponentArray[20];
} struct_pairing;

struct_message incomingReadings;
struct_message outgoingSetpoints;
struct_pairing pairingData;
struct_command commandData;

int pairingCount = 0;
int componentCount = 0;
String formatMAC(const uint8_t *mac_addr) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  return macStr;
}

void readMacAddress() {
  uint8_t baseMac[6];
  esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
  if (ret == ESP_OK) {
    Serial.println(formatMAC(baseMac));
    ServerMacAddress = formatMAC(baseMac);
  } else {
    Serial.println("Failed to read MAC address");
  }
}

void dataToSend(int switchIndex, int switchValue) {
  Serial.print(switchIndex);
  Serial.print(" - ");
  Serial.println(switchValue);

  for (JsonPair kv : doc.as<JsonObject>()) {
    JsonObject value = kv.value().as<JsonObject>();

    if (value.containsKey("component_type")) {
      if (strcmp("SWITCH_COMPONENT", value["component_type"].as<const char *>()) == 0) {
        if (value["index"].as<const uint8_t>() == switchIndex) {
          outgoingSetpoints.msgType = DATA;
          outgoingSetpoints.id = value["id"].as<const uint64_t>();
          outgoingSetpoints.component_type = SWITCH_COMPONENT;
          outgoingSetpoints.component_id = value["component_id"].as<const uint64_t>();
          outgoingSetpoints.component_value = switchValue;
          outgoingSetpoints.readingId = readingIdCounter++;
          Serial.print("SWITCH_COMPONENT send: "); 
          serializeJson(doc, Serial);        
          Serial.println();
          esp_now_send(NULL, (uint8_t *)&outgoingSetpoints, sizeof(outgoingSetpoints));
        }
      }
    }
  }
}

void readDataToSend() {
  // TESTING
  /*outgoingSetpoints.msgType = DATA;
  outgoingSetpoints.id = 0;
  outgoingSetpoints.component_type = 0;
  outgoingSetpoints.component_id = 0;
  outgoingSetpoints.component_value = 0;

  outgoingSetpoints.readingId = readingIdCounter++;  
  esp_now_send(NULL, (uint8_t *)&outgoingSetpoints, sizeof(outgoingSetpoints));
  */
  outgoingSetpoints.msgType = KEEPALIVE;
  esp_now_send(NULL, (uint8_t *)&outgoingSetpoints, sizeof(outgoingSetpoints));
}
// ---------------------------- esp_ now -------------------------
void printMAC(const uint8_t *mac_addr) {
  Serial.print(formatMAC(mac_addr));
}

bool addPeer(const uint8_t *peer_addr) {  // add pairing
  memset(&slave, 0, sizeof(slave));
  const esp_now_peer_info_t *peer = &slave;
  memcpy(slave.peer_addr, peer_addr, 6);

  slave.channel = chan;  // pick a channel
  slave.encrypt = 0;     // no encryption
  // check if the peer exists
  bool exists = esp_now_is_peer_exist(slave.peer_addr);
  if (exists) {
    // Slave already paired.
    //Serial.println("Already Paired");
    return true;
  } else {
    pairingCount++;
    esp_err_t addStatus = esp_now_add_peer(peer);
    if (addStatus == ESP_OK) {
      // Pair success
      //Serial.println("Pair success");
      return true;
    } else {
      //Serial.println("Pair failed");
      return false;
    }
  }
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("Last Packet Send Status: ");
  //Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success to " : "Delivery Fail to ");
  //printMAC(mac_addr);
  //Serial.println();
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
  //Serial.print(len);
  //Serial.println(" bytes of new data received.");

  StaticJsonDocument<1000> root;
  String payload;
  uint8_t type = incomingData[0];  // first message byte is the type of message
  String h_macStr;
  //JsonObject device;
  //const char *name = "";

  switch (type) {
    case DATA:  // the message is data type
      memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
      root["type"] = type;
      root["id"] = incomingReadings.id;
      root["component_type"] = incomingReadings.component_type;
      root["component_id"] = incomingReadings.component_id;
      root["component_value"] = incomingReadings.component_value;

      esp32ComponentType component_type;
      root["pairingCount"] = pairingCount;
      root["componentCount"] = componentCount;
      h_macStr = formatMAC(incomingReadings.macAddr);
      root["macAddr"] = h_macStr;
      root["readingId"] = String(incomingReadings.readingId);
      serializeJson(root, payload);

      char key[40];
      char serialAddres[40];
      sprintf(key, "%llu-%d", incomingReadings.id, incomingReadings.component_id);

      if (doc.containsKey(key)) {
        //device = doc[key];
      } else {
        JsonObject device;
        device = doc.createNestedObject(key);

        char *component_type = esp32ComponentTypeStr[incomingReadings.component_type];
        uint8_t index = doc[component_type].as<const uint8_t>();
        index = index + 1;
        doc[component_type] = index;

        device["index"] = index;
        device["component_type"] = component_type;
        device["id"] = incomingReadings.id;
        device["macAddr"] = h_macStr;
        device["component_id"] = incomingReadings.component_id;
        sprintf(key, "%llu-%d", incomingReadings.id, incomingReadings.component_id);
        device["key"] = key;
        sprintf(serialAddres, "zone_%d", index);
        device["name"] = serialAddres;
        storeConfig(doc);

        Serial.print("Store new device: "); 
        serializeJson(doc, Serial);        
        Serial.println();      
      }

      // Send message over UART

      if (incomingReadings.component_type == TEMPERATURE_COMPONENT) {
        StaticJsonDocument<1000> root = doc[key];
        root["temp"] = (float)incomingReadings.component_value;
        serializeJson(root, payload);
        events.send(payload.c_str(), "new_readings", millis());

        Serial.println(payload.c_str());
        mySerial.print(payload.c_str());
        mySerial.print("\n");        

        switch (root["index"].as<const uint8_t>()) {
          case 1:
            temperature1.update((float)incomingReadings.component_value);
            break;
          case 2:
            temperature2.update((float)incomingReadings.component_value);
            break;
          case 3:
            temperature3.update((float)incomingReadings.component_value);
            break;
          case 4:
            temperature4.update((float)incomingReadings.component_value);
            break;
          case 5:
            temperature5.update((float)incomingReadings.component_value);
            break;
          case 6:
            temperature6.update((float)incomingReadings.component_value);
            break;
          case 7:
            temperature7.update((float)incomingReadings.component_value);
            break;
          case 8:
            temperature8.update((float)incomingReadings.component_value);
            break;
          case 9:
            temperature9.update((float)incomingReadings.component_value);
            break;
          case 10:
            temperature10.update((float)incomingReadings.component_value);
            break;
          case 11:
            temperature11.update((float)incomingReadings.component_value);
            break;
          case 12:
            temperature12.update((float)incomingReadings.component_value);
            break;
        }
        dashboard.sendUpdates();
      }
      if (incomingReadings.component_type == SWITCH_COMPONENT) {
        StaticJsonDocument<1000> root = doc[key];
        root["switch"] = (int)incomingReadings.component_value;
        serializeJson(root, payload);
        events.send(payload.c_str(), "new_readings", millis());

        Serial.println(payload.c_str());
        mySerial.print(payload.c_str());
        mySerial.print("\n");
        
        switch (root["index"].as<const uint8_t>()) {
          case 1:
            button1.update((int)incomingReadings.component_value == 1);
            break;
          case 2:
            button2.update((int)incomingReadings.component_value == 1);
            break;
          case 3:
            button3.update((int)incomingReadings.component_value == 1);
            break;
          case 4:
            button4.update((int)incomingReadings.component_value == 1);
            break;
          case 5:
            button5.update((int)incomingReadings.component_value == 1);
            break;
          case 6:
            button6.update((int)incomingReadings.component_value == 1);
            break;
          case 7:
            button7.update((int)incomingReadings.component_value == 1);
            break;
          case 8:
            button8.update((int)incomingReadings.component_value == 1);
            break;
          case 9:
            button9.update((int)incomingReadings.component_value == 1);
            break;
          case 10:
            button10.update((int)incomingReadings.component_value == 1);
            break;
          case 11:
            button11.update((int)incomingReadings.component_value == 1);
            break;
          case 12:
            button12.update((int)incomingReadings.component_value == 1);
            break;
        }
        dashboard.sendUpdates();
      }
      break;
    case PAIRING:  // the message is a pairing request
      memcpy(&pairingData, incomingData, sizeof(pairingData));

      String macAddr = formatMAC(pairingData.macAddr);
      //Serial.print("Pairing request from MAC Address: ");
      //Serial.print(macAddr);
      //Serial.print(" on channel ");
      //Serial.println(pairingData.channel);

      clientMacAddress[0] = pairingData.macAddr[0];
      clientMacAddress[1] = pairingData.macAddr[1];
      clientMacAddress[2] = pairingData.macAddr[2];
      clientMacAddress[3] = pairingData.macAddr[3];
      clientMacAddress[4] = pairingData.macAddr[4];
      clientMacAddress[5] = pairingData.macAddr[5];

      root["type"] = type;
      root["msgType"] = pairingData.msgType;
      root["id"] = pairingData.id;
      root["macAddr"] = macAddr;
      root["esp32ComponentCount"] = pairingData.esp32ComponentCount;

      if (pairingData.id != 0) {  // do not replay to server itself
        if (pairingData.msgType == PAIRING) {
          for (int i = 0; i < pairingData.esp32ComponentCount; i++) {
            componentCount++;

            root["esp32Component"][i]["component_type"] = pairingData.esp32ComponentArray[i].component_type;
            root["esp32Component"][i]["component_id"] = pairingData.esp32ComponentArray[i].component_count;
          }

          pairingData.id = 0;  // 0 is server
          // Server is in AP_STA mode: peers need to send data to server soft AP MAC address
          WiFi.softAPmacAddress(pairingData.macAddr);
          pairingData.channel = chan;

          serializeJson(root, payload);
          //Serial.print("event send:");
          //serializeJson(root, Serial);
          //Serial.println();

          /*mySerial.println(payload.c_str());
          Serial.println("Send mySerial: ");
          Serial.println(payload.c_str());
          */
          addPeer(clientMacAddress);

          esp_err_t result = esp_now_send(clientMacAddress, (uint8_t *)&pairingData, sizeof(pairingData));
          //Serial.print("send response, result: ");
          //Serial.println(result);
        }
      }
      break;
  }
}

void initESP_NOW() {
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void setup_esp32() {
  mySerial.begin(115200, SERIAL_8N1, RXD2, TXD2);  // UART setup
  Serial.println("ESP32 UART Transmitter");

  WiFi.mode(WIFI_STA);
  WiFi.STA.begin();
  Serial.print("Server MAC Address: ");
  readMacAddress();

  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }

  Serial.print("Server SOFT AP MAC Address:  ");
  Serial.println(WiFi.softAPmacAddress());

  chan = WiFi.channel();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  initESP_NOW();
}

void mySerialLoop() {
  while (mySerial.available()) {
    char incomingChar = mySerial.read();
    if (incomingChar == '\n') {      
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, receivedMessage);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());      
      } else {
        Serial.print("Found msg: ");
        serializeJson(doc, Serial);
        Serial.println();

        if ((doc["msgType"].as<const uint8_t>() == DATA) && (doc["component_type"].as<const uint8_t>() == SWITCH_COMPONENT)) {
          dataToSend(doc["component_id"].as<const int>(), doc["component_value"].as<const int>());
        }
      }
      receivedMessage = "";
    } else {
      // Append the character to the message string
      receivedMessage += incomingChar;
    }
  }
}