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

    unsigned long previousMillis = 0;

    void FloorheatingComponent::setup()
    {
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
              if ((strcmp(name, "zone_11") == 0) && (this->tempzone10sensor != nullptr))
              {
                this->tempzone10sensor->publish_state((float)v);
              }
              if ((strcmp(name, "zone_12") == 0) && (this->tempzone10sensor != nullptr))
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
              if ((strcmp(name, "zone_11") == 0) && (this->switchzone10sensor != nullptr))
              {
                this->switchzone10sensor->publish_state(v);
              }
              if ((strcmp(name, "zone_12") == 0) && (this->switchzone10sensor != nullptr))
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

      if (millis() - previousMillis >= 5000)
      {
        char sendMessage[sendMessage_BUFFER_LEN];
        previousMillis = millis();

        ESP_LOGD(TAG, "internal_zone_1=%d, %d", this->switchzone1sensor->state, internal_zone_1);

        if (this->switchzone1sensor != nullptr)
        {
          if (this->switchzone1sensor->state != internal_zone_1)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":1, \"component_value\":%d}\n", internal_zone_1);
            this->write_str(sendMessage);
            this->flush();
            ESP_LOGD(TAG, "sendMessage=%s", sendMessage);
          }
        }
        if (this->switchzone2sensor != nullptr)
        {
          if (this->switchzone2sensor->state != internal_zone_2)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":2, \"component_value\":%d}\n", internal_zone_2);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone3sensor != nullptr)
        {
          if (this->switchzone3sensor->state != internal_zone_3)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":3, \"component_value\":%d}\n", internal_zone_3);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone4sensor != nullptr)
        {
          if (this->switchzone4sensor->state != internal_zone_4)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":4, \"component_value\":%d}\n", internal_zone_4);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone5sensor != nullptr)
        {
          if (this->switchzone5sensor->state != internal_zone_5)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":5, \"component_value\":%d}\n", internal_zone_5);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone6sensor != nullptr)
        {
          if (this->switchzone6sensor->state != internal_zone_6)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":6, \"component_value\":%d}\n", internal_zone_6);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone7sensor != nullptr)
        {
          if (this->switchzone7sensor->state != internal_zone_7)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":7, \"component_value\":%d}\n", internal_zone_7);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone8sensor != nullptr)
        {
          if (this->switchzone8sensor->state != internal_zone_8)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":8, \"component_value\":%d}\n", internal_zone_8);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone9sensor != nullptr)
        {
          if (this->switchzone9sensor->state != internal_zone_9)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":9, \"component_value\":%d}\n", internal_zone_9);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone10sensor != nullptr)
        {
          if (this->switchzone10sensor->state != internal_zone_10)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":10, \"component_value\":%d}\n", internal_zone_10);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone11sensor != nullptr)
        {
          if (this->switchzone11sensor->state != internal_zone_11)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":11, \"component_value\":%d}\n", internal_zone_11);
            this->write_str(sendMessage);
            this->flush();
          }
        }
        if (this->switchzone12sensor != nullptr)
        {
          if (this->switchzone12sensor->state != internal_zone_12)
          {
            sprintf(sendMessage, "{\"msgType\":1,\"component_type\":2, \"component_id\":12, \"component_value\":%d}\n", internal_zone_12);
            this->write_str(sendMessage);
            this->flush();
          }
        }
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
