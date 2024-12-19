#pragma once

#include "esphome/components/climate/climate.h"
#include "esphome/components/sensor/sensor.h"
#include "../floorheating_component/floorheating_component.h"

namespace esphome
{
    namespace floorheating_climate
    {

        // Temperature
        const uint8_t FH_TEMP_MIN = 15; // Celsius
        const uint8_t FH_TEMP_MAX = 30; // Celsius

        using climate::ClimateCall;
        using climate::ClimateMode;
        using climate::ClimatePreset;
        using climate::ClimateTraits;

        class FloorheatingClimate : public climate::Climate, public Component
        {
        public:
            FloorheatingClimate() { this->init(); }
            void control(const climate::ClimateCall &call) override;
            void loop() override;
            void setup() override;
            void init();
            esphome::climate::ClimateTraits traits() override;
            void set_sensor(sensor::Sensor *sensor) { this->sensor_ = sensor; }
            void set_floorheatingcomponent(floorheatingcomponent::FloorheatingComponent *fh) { this->parent = fh; }

            void processUpdate(float temp);
            void publishUpdate();
            void set_zones(int zone, bool status);
            void update_zone(uint16_t state);

        private:
            int errCounter = 0;
            bool zone1 = false;
            bool zone2 = false;
            bool zone3 = false;
            bool zone4 = false;
            bool zone5 = false;
            bool zone6 = false;
            bool zone7 = false;
            bool zone8 = false;
            bool zone9 = false;
            bool zone10 = false;
            bool zone11 = false;
            bool zone12 = false;
        protected:
            int status = 0;
            float target_temperature_ = 0;
            unsigned long connectionStart = 0;
            unsigned long previousMillis = 0;
            const long interval = 60000;
            sensor::Sensor *sensor_{nullptr};            

            floorheatingcomponent::FloorheatingComponent *parent{nullptr};
        };
    }
}