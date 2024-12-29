/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <esp_config.h>
#include <esp32_sender.h>

static bool DEBUG_TEMP = true;

// Data wire is plugged TO GPIO 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Number of temperature devices found
int numberOfDevices;
int deviceCount = 0;
//struct_ESP32_Component_Type esp32ComponentArray[20];

struct_message sendMyData;  // data to send
// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress;

bool storeComponentArray(DeviceAddress deviceAddress) {
  bool changed = false;
  char macStr[20];
  snprintf(macStr, sizeof(macStr), "%02x%02x%02x%02x%02x%02x%02x%02x",
           deviceAddress[0], deviceAddress[1], deviceAddress[2], deviceAddress[3], deviceAddress[4], deviceAddress[5], deviceAddress[6], deviceAddress[7]);
  
  String h_macStr = macStr;
  
  if (doc["devices"][h_macStr].is<int>()) {    
    int32_t id = doc["devices"][h_macStr];
  } else {
    deviceCount++;
    devices[h_macStr] = deviceCount;    
    changed = true;
  }
  return changed;
}

void setup() {
  doc = readConfig();    
  if (DEBUG_TEMP) {
    Serial.begin(115200);
    Serial.println("esp32_esnow_temp_v20");
    Serial.println("start doc: ");
    serializeJson(doc, Serial);
  }

  if (doc["interval"].is<long>()) {
    send_interval = doc["interval"];
  } else {    
    doc["interval"] = send_interval;
    storeConfig(doc);
  }
  doc["interval"] = send_interval;
  storeConfig(doc);

  devices = doc.createNestedObject("devices");

  // Start up the library
  sensors.begin();

  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  if (DEBUG_TEMP) {
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(numberOfDevices, DEC);
    Serial.println(" devices.");
  }

  // Loop through each device, print out address
  bool changed = false;
  for (int i = 0; i < numberOfDevices; i++) {
     // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i)) {
      storeComponentArray(tempDeviceAddress);
      changed = true;
    } else {
      if (DEBUG_TEMP) {
        Serial.print("Found ghost device at ");
        Serial.print(i, DEC);
        Serial.print(" but could not detect address. Check power and cabling");
      }
    }    
  }  
  esp_now_setup(numberOfDevices, TEMPERATURE_COMPONENT);
  if (DEBUG_TEMP) {
    serializeJson(doc, Serial);
    Serial.println();    
  }
  if (changed) {
    storeConfig(doc);
  }
  delay(10000);
}

void loop() {
  int32_t id;
  char macStr[20];
  if (autoPairing() == PAIR_PAIRED) {
    sensors.requestTemperatures();  // Send the command to get temperatures
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= send_interval) {
      previousMillis = currentMillis;

      serializeJson(doc, Serial);
      Serial.println();   

      // Loop through each device, print out temperature data
      for (int i = 0; i < numberOfDevices; i++) {
        // Search the wire for address
        if (sensors.getAddress(tempDeviceAddress, i)) {
          snprintf(macStr, sizeof(macStr), "%02x%02x%02x%02x%02x%02x%02x%02x",
                   tempDeviceAddress[0], tempDeviceAddress[1], tempDeviceAddress[2], tempDeviceAddress[3], tempDeviceAddress[4], tempDeviceAddress[5], tempDeviceAddress[6], tempDeviceAddress[7]);

          String h_macStr = macStr;

          if (!(doc["devices"][h_macStr].is<int>())) {
            storeComponentArray(tempDeviceAddress);
            storeConfig(doc);                        
          }

          id = doc["devices"][h_macStr];

          float tempC = sensors.getTempC(tempDeviceAddress);

          if (DEBUG_TEMP) {
            Serial.print("Temperature for device: ");
            Serial.print(i, DEC);
            Serial.print(" with address: ");
            Serial.print(macStr);
            Serial.print(" id: ");
            Serial.print(id);

            // Print the data
            Serial.print(" Temp C: ");
            Serial.println(tempC);
          }
          // Send message via ESP-NOW to all peers
          sendMyData.msgType = DATA;
          sendMyData.component_type = TEMPERATURE_COMPONENT;
          sendMyData.component_id = id;
          sendMyData.component_value = tempC;
          esp_now_20_send(sendMyData);
        } 
      }
    }
  }
  esp_loop();
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }  
}