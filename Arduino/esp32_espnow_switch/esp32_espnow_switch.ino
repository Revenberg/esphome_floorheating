/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-auto-pairing-esp32-esp8266/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based on JC Servaye example: https://https://github.com/Servayejc/esp8266_espnow
*/
#include "C:\Users\reven\OneDrive\Documenten\Arduino\esp32_espnow_switch\esp8266_sender.h"

static unsigned long lastSendEventTime = millis();

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  setup_esp8266();
}

void loop() {
  if (autoPairing() == PAIR_PAIRED) {       
    if ((millis() - lastSendEventTime) > send_interval) {
      //Set values to send
      myData.msgType = DATA;
      myData.id = board_id;
      myData.macAddr[0] = clientMacAddress[0];
      myData.macAddr[1] = clientMacAddress[1];
      myData.macAddr[2] = clientMacAddress[2];
      myData.macAddr[3] = clientMacAddress[3];
      myData.macAddr[4] = clientMacAddress[4];
      myData.macAddr[5] = clientMacAddress[5];

      // Send message via ESP-NOW to all peers
      myData.component_type = SWITCH_COMPONENT;
      myData.component_id = 1;
      myData.component_value = relay1_status;
      myData.readingId = readingId++;
      esp_now_send(pairingData.macAddr, (uint8_t *)&myData, sizeof(myData));

      // Send message via ESP-NOW to all peers
      myData.component_type = SWITCH_COMPONENT;
      myData.component_id = 2;
      myData.component_value = relay2_status;
      myData.readingId = readingId++;
      esp_now_send(pairingData.macAddr, (uint8_t *)&myData, sizeof(myData));

      lastSendEventTime = millis();
    }
  }
  esp_loop();
}