#include "ESPAsyncWebServer.h"
#include <ArduinoJson.h>

JsonDocument doc;

AsyncWebServer adminserver(8080);
AsyncEventSource events("/");

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE HTML>
<html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">  
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.temperature { color: #fd7e14; }
    .card.switch { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <form name="myform" id="myform" action="/get">
    <div class="content">
      <div class="cards">
        <div id="myHTMLWrapper"></div>        
      </div>      
    </div>    
    <input name="tempcount" type="hidden" value="">
    <input name="switchcount" type="hidden" value="">    
    <input name="field" type="hidden" value="">
    <input name="value" type="hidden" value="">    

    <a href="/refresh" class="button" >Refresh Page</a>

  </form>
<script>
function DoSubmit(id) {  
  console.log("id_"+ id);
  console.log("name_"+ id);

  document.forms[0].field.value = document.getElementById("id_"+ id).innerHTML;
  document.forms[0].value.value = document.getElementById("name_"+ id).value;
  document.getElementById("myform").submit();
}

function findGetParameter(parameterName) {
  var result = null,
      tmp = [];
  location.search
      .substr(1)
      .split("&")
      .forEach(function (item) {
        tmp = item.split("=");
        if (tmp[0] === parameterName) result = decodeURIComponent(tmp[1]);
      });
  return result;
}

const tempcount = findGetParameter('tempcount')
const switchcount = findGetParameter('switchcount')
var wrapper = document.getElementById("myHTMLWrapper");

var myHTML = "";
document.forms[0].tempcount.value = tempcount;
document.forms[0].switchcount.value = switchcount;

for(var i=1; i<=tempcount; i++) {
  myHTML += `<div class="card temperature">
                <h4><i class="fas fa-thermometer-half"></i>                
                <span id="temp_TEMPERATURE_COMPONENT_${i}"></span>&nbsp; 
                <span id="componenttype_TEMPERATURE_COMPONENT_${i}"></span>&nbsp;               
                <span id="id_TEMPERATURE_COMPONENT_${i}"></span>                                
                </h4>
                <p><span class="reading">
                  <p class="packet">Mac adres: <span id="rt_TEMPERATURE_COMPONENT_${i}"></span></p>
                </span>
                </p>
                Naam: <input type="text" id="name_TEMPERATURE_COMPONENT_${i}" onchange="DoSubmit('TEMPERATURE_COMPONENT_${i}')" />
              </div>`;
  }
  for(var i=1; i<=switchcount; i++) {
    myHTML += `<div class="card temperature">
                <h4><i class="fa fa-check-square-o"></i>                
                <span id="switch_SWITCH_COMPONENT_${i}"></span>&nbsp;
                <span id="componenttype_SWITCH_COMPONENT_${i}"></span>&nbsp;
                <span id="id_SWITCH_COMPONENT_${i}"></span>&nbsp;                
                </h4>
                <p><span class="reading">
                  <p class="packet">Mac adres: <span id="rt_SWITCH_COMPONENT_${i}"></span></p>
                </span>
                </p>
                Naam: <input type="text" id="name_SWITCH_COMPONENT_${i}" onchange="DoSubmit(SWITCH_COMPONENT_${i})" />
              </div>`;
  }
  wrapper.innerHTML = myHTML;

 if (!!window.EventSource) {
 var source = new EventSource('/');

 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);

 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);

 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  if (document.getElementById("id_" + obj.component_type + "_" +obj.index) == null) {
    console.log("index (id_" + obj.component_type + "_" + obj.index + ") not found");
  } else 
  {
    document.getElementById("id_" + obj.component_type + "_" +obj.index).innerHTML = obj.key;    
    // document.getElementById("key_" + obj.component_type + "_" +obj.index).innerHTML = obj.key;
    document.getElementById("rt_" + obj.component_type + "_" +obj.index).innerHTML = obj.macAddr;
    document.getElementById("name_" + obj.component_type + "_" +obj.index).value = obj.name;    
    
    if ((obj.component_type == "TEMPERATURE_COMPONENT") && (typeof obj.temp !== 'undefined')) {      
      document.getElementById("temp_" + obj.component_type + "_" +obj.index).innerHTML = obj.temp;
    } 
    if ((obj.component_type == "SWITCH_COMPONENT") && (typeof obj.switch !== 'undefined')) {      
      document.getElementById("switch_" + obj.component_type + "_" +obj.index).innerHTML = obj.switch;
    }
  }
 }, false);
}
</script>
</body>
</html>)rawliteral";

void updateConfig(String inputParam, String inputMessage) {
  Serial.println(inputParam);
  Serial.println(inputMessage);
  serializeJson(doc, Serial);

  if (doc.containsKey(inputParam)) {
    doc[inputParam]["name"] = inputMessage;    
    storeConfig(doc);
  }
}

//void webAdminSetup(JsonDocument doc) {
  void webAdminSetup() {
  StaticJsonDocument<1000> root;
  String payload;
  //int i = 0;

  // Start Web adminserver
  adminserver.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  adminserver.on("/firmware", HTTP_GET, [](AsyncWebServerRequest* request) {
    removeConfig();
  });

  adminserver.on("/refresh", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("refresh");
    char* component_type = esp32ComponentTypeStr[0];
    uint8_t tempcount = doc[component_type].as<const uint8_t>();

    component_type = esp32ComponentTypeStr[2];
    uint8_t switchcount = doc[component_type].as<const uint8_t>();

    char url[80];
    sprintf(url, "/?tempcount=%d&switchcount=%d", tempcount, switchcount);
    request->redirect(url);  
  });

  adminserver.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    String inputParam;
    //String tempcount;
    //String switchcount;

    int params = request->params();
    Serial.printf("%d params sent in\n", params);
    for (int i = 0; i < params; i++) {
      const AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        Serial.printf("_POST[%s]: %s", p->name().c_str(), p->value().c_str());
      } else {
        Serial.printf("_GET(%s): [%s]", p->name().c_str(), p->value().c_str());
        Serial.println();

        if (strcmp("field", p->name().c_str()) == 0) {
          inputParam = p->value().c_str();
        }
        if (strcmp("value", p->name().c_str()) == 0) {
          inputMessage = p->value().c_str();
        }
      }
    }

    updateConfig(inputParam, inputMessage);

    char* component_type = esp32ComponentTypeStr[0];
    uint8_t tempcount = doc[component_type].as<const uint8_t>();

    component_type = esp32ComponentTypeStr[2];
    uint8_t switchcount = doc[component_type].as<const uint8_t>();

    //request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" + inputParam + ") with value: " + inputMessage + "<br><a href=\"/admin?tempcount=" + String(tempcount) + "&" + "switchcount=" + String(switchcount) + "\">Return to Admin Page</a>");
    
    char url[80];
    sprintf(url, "/?tempcount=%d&switchcount=%d", tempcount, switchcount);    
//    server.sendHeader("Location","/admin?tempcount=" + String(tempcount) + "&" + "switchcount=" + String(switchcount));
    request->redirect(url);  
  });

  // Events
  events.onConnect([](AsyncEventSourceClient* client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  adminserver.addHandler(&events);
  // start adminserver
  adminserver.begin();
}

void updateAdmin(JsonDocument doc) {
  StaticJsonDocument<1000> root;
  String payload;  

  for (JsonPair kv : doc.as<JsonObject>()) {
    JsonObject value = kv.value().as<JsonObject>();
    if (value["index"].as<const uint8_t>() > 0) {
      root["key"] = kv.key().c_str();
      root["index"] = value["index"].as<const uint8_t>();
      root["name"] = value["name"].as<const char*>();
      root["id"] = value["id"].as<const uint64_t>();
      root["component_type"] = value["component_type"].as<const char*>();
      root["macAddr"] = value["macAddr"].as<const char*>();
      serializeJson(root, payload);
      events.send(payload.c_str(), "new_readings", millis());      
    }
  }
}