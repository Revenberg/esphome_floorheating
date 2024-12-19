#include "floorheating_climate.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include <iostream>

namespace esphome
{
    namespace floorheating_climate
    {
        static const char *const TAG = "floorheating_climate.climate";

        void FloorheatingClimate::init()
        {
            this->target_temperature = 22;
            this->publish_state();
        }

        void FloorheatingClimate::setup()
        {

            this->sensor_->add_on_state_callback([this](float state)                                                 {
                this->current_temperature = state;
                this->publish_state(); });
            this->current_temperature = this->sensor_->state;
            this->publish_state();
        }

        ClimateTraits FloorheatingClimate::traits()
        {
            auto traits = esphome::climate::ClimateTraits();
            traits.set_supports_action(false);

            traits.set_supports_current_temperature(true);
            traits.set_visual_min_temperature(16);
            traits.set_visual_max_temperature(30);
            traits.set_visual_temperature_step(0.1);

            traits.add_supported_mode(ClimateMode::CLIMATE_MODE_HEAT);

            return traits;
        }
        void FloorheatingClimate::processUpdate(float temp)
        {
            this->target_temperature = temp;
            this->publishUpdate();
        }
        void FloorheatingClimate::set_zones(int zone, bool status)
        {
            switch (zone)
            {
            case 1:
                zone1 = status;
                break;
            case 2:
                zone2 = status;
                break;
            case 3:
                zone3 = status;
                break;
            case 4:
                zone4 = status;
                break;
            case 5:
                zone5 = status;
                break;
            case 6:
                zone6 = status;
                break;
            case 7:
                zone7 = status;
                break;
            case 8:
                zone8 = status;
                break;
            case 9:
                zone9 = status;
                break;
            case 10:
                zone10 = status;
                break;
            case 11:
                zone11 = status;
                break;
            case 12:
                zone12 = status;
                break;
            }
        }
        void FloorheatingClimate::update_zone(uint16_t state)
        {
            if (zone1)
                this->parent->set_output(1, state);
            if (zone2)
                this->parent->set_output(2, state);
            if (zone3)
                this->parent->set_output(3, state);
            if (zone4)
                this->parent->set_output(4, state);
            if (zone5)
                this->parent->set_output(5, state);
            if (zone6)
                this->parent->set_output(6, state);
            if (zone7)
                this->parent->set_output(7, state);
            if (zone8)
                this->parent->set_output(8, state);
            if (zone9)
                this->parent->set_output(9, state);
            if (zone10)
                this->parent->set_output(10, state);
            if (zone11)
                this->parent->set_output(11, state);
            if (zone12)
                this->parent->set_output(12, state);
        }

        void FloorheatingClimate::publishUpdate()
        {
            climate::ClimateAction target_action;

            if (this->current_temperature < this->target_temperature)
            {
                target_action = climate::CLIMATE_ACTION_HEATING;
                this->mode = climate::CLIMATE_MODE_HEAT;
                this->update_zone(true);
            }
            else
            {
                target_action = climate::CLIMATE_ACTION_IDLE;
                this->mode = climate::CLIMATE_MODE_OFF;
                this->update_zone(false);
            }
            if (target_action != this->action)
            {
                this->action = target_action;
                this->publish_state();
            }
        }

        void FloorheatingClimate::loop()
        {
            if (millis() - previousMillis >= 1000)
            {
                previousMillis = millis();
                this->publishUpdate();
            }
        }

        void FloorheatingClimate::control(const climate::ClimateCall &call)
        {
            if (call.get_mode().has_value())
            {
                ClimateMode mode = *call.get_mode();
                this->mode = mode;
                this->publish_state();
            }
            if (call.get_target_temperature().has_value())
            {
                float temp = *call.get_target_temperature();
                this->target_temperature = temp;
            }
            this->target_temperature = (uint8_t)roundf(clamp<float>(this->target_temperature, FH_TEMP_MIN, FH_TEMP_MAX));
            this->publish_state();
        }
    }
}