#include <esp_config.h>
#include <webadmin.h>
#include <dashboard.h>
#include <esp32_sender.h>
#include <vector>

void setup() {  
  // Initialize Serial Monitor
  Serial.begin(115200); 
  Serial.println("esp32_espnow_V20");   
  //storeConfig(doc); 
  doc = readConfig();     

  for (byte idx = 0; idx < sizeof(esp32ComponentTypeStr) / sizeof(esp32ComponentTypeStr[0]); idx++) {
    if (not(doc.containsKey(esp32ComponentTypeStr[idx]))) {                
      doc[esp32ComponentTypeStr[idx]] = 0;
    }
  }
  setup_esp32(); 
  dashboardSetup();
  webAdminSetup();    
}

void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 15000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    lastEventTime = millis();
    readDataToSend();        
    updateAdmin(doc);    
  }
  if (switchIndex > 0) {    
    dataToSend(switchIndex, switchValue);
    switchIndex = 0;    
  }
  mySerialLoop();
}