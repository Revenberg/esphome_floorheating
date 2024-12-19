#include <ESPDash.h>

/* Attach ESP-DASH to AsyncWebServer */
#include <ESPAsyncWebServer.h>

int switchIndex = 0;
int switchValue = 0;

AsyncWebServer server(80);
ESPDash dashboard(&server, "/", false);

Card temperature1(&dashboard, TEMPERATURE_CARD, "Temperature 1", "°C");
Card button1(&dashboard, BUTTON_CARD, "Switch 1", "");

Card temperature2(&dashboard, TEMPERATURE_CARD, "Temperature 2", "°C");
Card button2(&dashboard, BUTTON_CARD, "Switch 2", "");

Card temperature3(&dashboard, TEMPERATURE_CARD, "Temperature 3", "°C");
Card button3(&dashboard, BUTTON_CARD, "Switch 3", "");

Card temperature4(&dashboard, TEMPERATURE_CARD, "Temperature 4", "°C");
Card button4(&dashboard, BUTTON_CARD, "Switch 4", "");

Card temperature5(&dashboard, TEMPERATURE_CARD, "Temperature 5", "°C");
Card button5(&dashboard, BUTTON_CARD, "Switch 5", "");

Card temperature6(&dashboard, TEMPERATURE_CARD, "Temperature 6", "°C");
Card button6(&dashboard, BUTTON_CARD, "Switch 6", "");

Card temperature7(&dashboard, TEMPERATURE_CARD, "Temperature 7", "°C");
Card button7(&dashboard, BUTTON_CARD, "Switch 7", "");

Card temperature8(&dashboard, TEMPERATURE_CARD, "Temperature 8", "°C");
Card button8(&dashboard, BUTTON_CARD, "Switch 8", "");

Card temperature9(&dashboard, TEMPERATURE_CARD, "Temperature 9", "°C");
Card button9(&dashboard, BUTTON_CARD, "Switch 9", "");

Card temperature10(&dashboard, TEMPERATURE_CARD, "Temperature 10", "°C");
//Card id10(&dashboard, GENERIC_CARD, "id 10");
Card button10(&dashboard, BUTTON_CARD, "Switch 10", "");

Card temperature11(&dashboard, TEMPERATURE_CARD, "Temperature 11", "°C");
Card button11(&dashboard, BUTTON_CARD, "Switch 11", "");

Card temperature12(&dashboard, TEMPERATURE_CARD, "Temperature 12", "°C");
Card button12(&dashboard, BUTTON_CARD, "Switch 12", "");

void dashboardSetup() {
  Serial.begin(115200);
  button1.attachCallback([&](int value) {
    Serial.println("[button2] Callback Triggered: "+String(value));
    button1.update(value);    
    switchIndex = 1;
    switchValue = value;
    dashboard.sendUpdates();
  });    
  button2.attachCallback([&](int value){
    Serial.println("[button2] Callback Triggered: "+String(value));
    button2.update(value);
    switchIndex = 2;
    switchValue = value;
    dashboard.sendUpdates();
  });
  button3.attachCallback([&](int value){
    Serial.println("[button3] Callback Triggered: "+String(value));
    button3.update(value);
    switchIndex = 3;
    switchValue = value;
    dashboard.sendUpdates();
  });  
  button4.attachCallback([&](int value){
    Serial.println("[button4] Callback Triggered: "+String(value));
    button4.update(value);
    switchIndex = 4;
    switchValue = value;
    dashboard.sendUpdates();
  });  
  button5.attachCallback([&](int value){
    Serial.println("[button5] Callback Triggered: "+String(value));
    button5.update(value);
    switchIndex = 5;
    switchValue = value;
    dashboard.sendUpdates();
  });  
  button6.attachCallback([&](int value){
    Serial.println("[button6] Callback Triggered: "+String(value));
    button6.update(value);
    switchIndex = 6;
    switchValue = value;
    dashboard.sendUpdates();
  });
  button7.attachCallback([&](int value){
    Serial.println("[button7] Callback Triggered: "+String(value));
    button7.update(value);
    switchIndex = 7;
    switchValue = value;
    dashboard.sendUpdates();
  });
  button8.attachCallback([&](int value){
    Serial.println("[button8] Callback Triggered: "+String(value));
    button8.update(value);
    switchIndex = 8;
    switchValue = value;
    dashboard.sendUpdates();
  });  
  button9.attachCallback([&](int value){
    Serial.println("[button9] Callback Triggered: "+String(value));
    button9.update(value);
    switchIndex = 9;
    switchValue = value;
    dashboard.sendUpdates();
  });  
  button10.attachCallback([&](int value){
    Serial.println("[button10] Callback Triggered: "+String(value));
    button10.update(value);
    switchIndex = 10;
    switchValue = value;
    dashboard.sendUpdates();
  }); 
  button11.attachCallback([&](int value){
    Serial.println("[button11] Callback Triggered: "+String(value));
    button11.update(value);
    switchIndex = 11;
    switchValue = value;
    dashboard.sendUpdates();
  }); 
  button12.attachCallback([&](int value){
    Serial.println("[button12] Callback Triggered: "+String(value));
    button12.update(value);
    switchIndex = 12;
    switchValue = value;
    dashboard.sendUpdates();
  });    
  // start server    
  server.begin();
  Serial.println("server.begin");    
}