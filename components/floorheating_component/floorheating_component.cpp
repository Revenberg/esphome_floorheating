#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/log.h"
#include "floorheating_component.h"
#include <ArduinoJson.h>

namespace esphome
{
  namespace floorheatingcomponent
  {

    static const char *TAG = "floorheating";
    String receivedMessage = "";
    char sendMessage[sendMessage_BUFFER_LEN];

    void FloorheatingComponent::setup()
    {
      // nothing to do here
      receivedMessage = "";
      memset(sendMessage, 0, sendMessage_BUFFER_LEN * (sizeof sendMessage[0]) );     
      ESP_LOGD(TAG, "message send=%s", sendMessage); 
    }

    void FloorheatingComponent::loop()
    {
      while (this->available())
      {
        char incomingChar = this->read();        
        
        if (incomingChar == '\n')        
        { // Check if the user pressed Enter (new line character)

          ESP_LOGD(TAG, "message received=%s", receivedMessage.c_str());

          DynamicJsonDocument doc(1000);

          auto error = deserializeJson(doc, receivedMessage);
          if (error)
          {
            ESP_LOGE(TAG, "deserializeJson() failed with code: %s", error.c_str());
            receivedMessage = "";
            return;
          }

          if (doc.containsKey("component_type"))
          {
            const char *component_type = doc["component_type"].as<const char *>();
            const char *name = doc["name"].as<const char *>();
            if (strcmp(component_type, "TEMPERATURE_COMPONENT") == 0)
            {
              float v = doc["temp"].as<const float>();
              if ((strcmp(name, "zone_1") == 0) && (this->tempzone1sensor != nullptr))
              {
                this->tempzone1sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_2") == 0) && (this->tempzone2sensor != nullptr))
              {
                this->tempzone2sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_3") == 0) && (this->tempzone3sensor != nullptr))
              {
                this->tempzone3sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_4") == 0) && (this->tempzone4sensor != nullptr))
              {
                this->tempzone4sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_5") == 0) && (this->tempzone5sensor != nullptr))
              {
                this->tempzone5sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_6") == 0) && (this->tempzone6sensor != nullptr))
              {
                this->tempzone6sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_7") == 0) && (this->tempzone7sensor != nullptr))
              {
                this->tempzone7sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_8") == 0) && (this->tempzone8sensor != nullptr))
              {
                this->tempzone8sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_9") == 0) && (this->tempzone9sensor != nullptr))
              {
                this->tempzone9sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_10") == 0) && (this->tempzone10sensor != nullptr))
              {
                this->tempzone10sensor->publish_state((float)v);
              }
            }

            if (strcmp(component_type, "SWITCH_COMPONENT") == 0)
            {
              int v = doc["switch"].as<const int>();
              if ((strcmp(name, "zone_1") == 0) && (this->switchzone1sensor != nullptr))
              {
                this->switchzone1sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_2") == 0) && (this->switchzone2sensor != nullptr))
              {
                this->switchzone2sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_3") == 0) && (this->switchzone3sensor != nullptr))
              {
                this->switchzone3sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_4") == 0) && (this->switchzone4sensor != nullptr))
              {
                this->switchzone4sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_5") == 0) && (this->switchzone5sensor != nullptr))
              {
                this->switchzone5sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_6") == 0) && (this->switchzone6sensor != nullptr))
              {
                this->switchzone6sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_7") == 0) && (this->switchzone7sensor != nullptr))
              {
                this->switchzone7sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_8") == 0) && (this->switchzone8sensor != nullptr))
              {
                this->switchzone8sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_9") == 0) && (this->switchzone9sensor != nullptr))
              {
                this->switchzone9sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_10") == 0) && (this->switchzone10sensor != nullptr))
              {
                this->switchzone10sensor->publish_state(v);
              }
            }
          }
          // Clear the message buffer for the next input
          receivedMessage = "";
        }
        else
        {
          // Append the character to the message string
          receivedMessage += incomingChar;
        }
      }
      if (sendMessage != "") {
          this->write_str(sendMessage);
          this->flush();
          memset(sendMessage, 0, sendMessage_BUFFER_LEN * (sizeof sendMessage[0]) );    
      }
      
    }
    void FloorheatingComponent::update()
    {
    }

    void FloorheatingComponent::dump_config()
    {
      ESP_LOGCONFIG(TAG, "FloorheatingComponent Component");
    }

    bool FloorheatingComponent::set_output(uint16_t zone, bool state)
    {      
      sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":%d, \"component_value\": %d}\n", zone, state);

      return state;
    }

  } // namespace floorheatingcomponent
} // namespace esphome
