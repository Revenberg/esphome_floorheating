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

    void FloorheatingComponent::setup()
    {
      previousMillis = 0;
      previousSendMillis = 0;
    }

    void FloorheatingComponent::loop()
    {
      char sendMessage[sendMessage_BUFFER_LEN];

      while (this->available())
      {
        char incomingChar = this->read();

        if (incomingChar == '\n')
        { // Check if the user pressed Enter (new line character)

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

            ESP_LOGD(TAG, "component_type=%s, name%s", component_type, name);
            
            if (strcmp(component_type, "TEMPERATURE_COMPONENT") == 0)
            {
              ESP_LOGD(TAG, "message temp received=%s", receivedMessage.c_str());

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
              if ((strcmp(name, "zone_11") == 0) && (this->tempzone11sensor != nullptr))
              {
                this->tempzone11sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_12") == 0) && (this->tempzone12sensor != nullptr))
              {
                this->tempzone12sensor->publish_state((float)v);
              }
            }

            if (strcmp(component_type, "SWITCH_COMPONENT") == 0)
            {
              ESP_LOGD(TAG, "message SWITCH received=%s", receivedMessage.c_str());

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
              if ((strcmp(name, "zone_11") == 0) && (this->switchzone11sensor != nullptr))
              {
                this->switchzone11sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_12") == 0) && (this->switchzone12sensor != nullptr))
              {
                this->switchzone12sensor->publish_state(v);
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
      
      if (millis() - previousMillis >= 1000)
      {        
        previousMillis = millis();
        counter--;
        if (counter <= 0)
        {
          counter = 12;
          resend = millis() - previousSendMillis >= 60000;
          previousSendMillis = millis();
        }        
        switch (counter)
        {
        case 1:
          if (this->switchzone1sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone1sensor, internal_zone_1);
          break;
        case 2:
          if (this->switchzone2sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone2sensor, internal_zone_2);
          break;
        case 3:
          if (this->switchzone3sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone3sensor, internal_zone_3);
          break;
        case 4:
          if (this->switchzone4sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone4sensor, internal_zone_4);
          break;
        case 5:
          if (this->switchzone5sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone5sensor, internal_zone_5);
          break;
        case 6:
          if (this->switchzone6sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone6sensor, internal_zone_6);
          break;
        case 7:
          if (this->switchzone7sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone7sensor, internal_zone_7);
          break;
        case 8:
          if (this->switchzone8sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone8sensor, internal_zone_8);
          break;
        case 9:
          if (this->switchzone9sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone9sensor, internal_zone_9);
          break;
        case 10:
          if (this->switchzone10sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone10sensor, internal_zone_10);
          break;
        case 11:
          if (this->switchzone11sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone11sensor, internal_zone_11);
          break;
        case 12:
          if (this->switchzone12sensor != nullptr)
            FloorheatingComponent::writeToSerial(resend, counter, this->switchzone12sensor, internal_zone_12);
          break;
        }
      }
    }

    void FloorheatingComponent::writeToSerial(bool resend, unsigned long component_id, binary_sensor::BinarySensor *switchzonesensor, bool status)
    {      
      if ((switchzonesensor->state != status) || (resend))
      {
        char sendMessage[sendMessage_BUFFER_LEN];
        sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":%d, \"component_value\":%d}\n", component_id, status);
        this->write_str(sendMessage);
        this->flush();
        ESP_LOGD(TAG, "sendMessage=%s", sendMessage);
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
      switch (zone)
      {
      case 1:
        internal_zone_1 = state;
        break;
      case 2:
        internal_zone_2 = state;
        break;
      case 3:
        internal_zone_3 = state;
        break;
      case 4:
        internal_zone_4 = state;
        break;
      case 5:
        internal_zone_5 = state;
        break;
      case 6:
        internal_zone_6 = state;
        break;
      case 7:
        internal_zone_7 = state;
        break;
      case 8:
        internal_zone_8 = state;
        break;
      case 9:
        internal_zone_9 = state;
        break;
      case 10:
        internal_zone_10 = state;
        break;
      case 11:
        internal_zone_11 = state;
        break;
      case 12:
        internal_zone_12 = state;
        break;
      }
      return 0;
    }

  } // namespace floorheatingcomponent
} // namespace esphome
