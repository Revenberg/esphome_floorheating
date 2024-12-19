#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/uart/uart.h"

#define sendMessage_BUFFER_LEN 150

namespace esphome
{
  namespace floorheatingcomponent
  {

    class FloorheatingComponent : public PollingComponent, public uart::UARTDevice
    {
    public:
      void setup() override;
      void loop() override;
      void update() override;
      void dump_config() override;
      bool set_output(uint16_t zone, bool state);

      void set_temp_zone_1_sensor(sensor::Sensor *s) { tempzone1sensor = s; }
      void set_temp_zone_2_sensor(sensor::Sensor *s) { tempzone2sensor = s; }
      void set_temp_zone_3_sensor(sensor::Sensor *s) { tempzone3sensor = s; }
      void set_temp_zone_4_sensor(sensor::Sensor *s) { tempzone4sensor = s; }
      void set_temp_zone_5_sensor(sensor::Sensor *s) { tempzone5sensor = s; }
      void set_temp_zone_6_sensor(sensor::Sensor *s) { tempzone6sensor = s; }
      void set_temp_zone_7_sensor(sensor::Sensor *s) { tempzone7sensor = s; }
      void set_temp_zone_8_sensor(sensor::Sensor *s) { tempzone8sensor = s; }
      void set_temp_zone_9_sensor(sensor::Sensor *s) { tempzone9sensor = s; }
      void set_temp_zone_10_sensor(sensor::Sensor *s) { tempzone10sensor = s; }
      void set_temp_zone_11_sensor(sensor::Sensor *s) { tempzone11sensor = s; }
      void set_temp_zone_12_sensor(sensor::Sensor *s) { tempzone12sensor = s; }

      void set_switch_zone_1_sensor(binary_sensor::BinarySensor *s) { switchzone1sensor = s; }
      void set_switch_zone_2_sensor(binary_sensor::BinarySensor *s) { switchzone2sensor = s; }
      void set_switch_zone_3_sensor(binary_sensor::BinarySensor *s) { switchzone3sensor = s; }
      void set_switch_zone_4_sensor(binary_sensor::BinarySensor *s) { switchzone4sensor = s; }
      void set_switch_zone_5_sensor(binary_sensor::BinarySensor *s) { switchzone5sensor = s; }
      void set_switch_zone_6_sensor(binary_sensor::BinarySensor *s) { switchzone6sensor = s; }
      void set_switch_zone_7_sensor(binary_sensor::BinarySensor *s) { switchzone7sensor = s; }
      void set_switch_zone_8_sensor(binary_sensor::BinarySensor *s) { switchzone8sensor = s; }
      void set_switch_zone_9_sensor(binary_sensor::BinarySensor *s) { switchzone9sensor = s; }
      void set_switch_zone_10_sensor(binary_sensor::BinarySensor *s) { switchzone10sensor = s; }
      void set_switch_zone_11_sensor(binary_sensor::BinarySensor *s) { switchzone11sensor = s; }
      void set_switch_zone_12_sensor(binary_sensor::BinarySensor *s) { switchzone12sensor = s; }

    protected:
      sensor::Sensor *tempzone1sensor;
      sensor::Sensor *tempzone2sensor;
      sensor::Sensor *tempzone3sensor;
      sensor::Sensor *tempzone4sensor;
      sensor::Sensor *tempzone5sensor;
      sensor::Sensor *tempzone6sensor;
      sensor::Sensor *tempzone7sensor;
      sensor::Sensor *tempzone8sensor;
      sensor::Sensor *tempzone9sensor;
      sensor::Sensor *tempzone10sensor;
      sensor::Sensor *tempzone11sensor;
      sensor::Sensor *tempzone12sensor;

      binary_sensor::BinarySensor *switchzone1sensor;
      binary_sensor::BinarySensor *switchzone2sensor;
      binary_sensor::BinarySensor *switchzone3sensor;
      binary_sensor::BinarySensor *switchzone4sensor;
      binary_sensor::BinarySensor *switchzone5sensor;
      binary_sensor::BinarySensor *switchzone6sensor;
      binary_sensor::BinarySensor *switchzone7sensor;
      binary_sensor::BinarySensor *switchzone8sensor;
      binary_sensor::BinarySensor *switchzone9sensor;
      binary_sensor::BinarySensor *switchzone10sensor;
      binary_sensor::BinarySensor *switchzone11sensor;
      binary_sensor::BinarySensor *switchzone12sensor;

      String receivedMessage = "";
      bool internal_zone_1 = false;
      bool internal_zone_2 = false;
      bool internal_zone_3 = false;
      bool internal_zone_4 = false;
      bool internal_zone_5 = false;
      bool internal_zone_6 = false;
      bool internal_zone_7 = false;
      bool internal_zone_8 = false;
      bool internal_zone_9 = false;
      bool internal_zone_10 = false;
      bool internal_zone_11 = false;
      bool internal_zone_12 = false;
    };

  } // namespace floorheatingcomponent
} // namespace esphome
