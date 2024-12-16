#include "esphome/core/log.h"
#include "floorheating_switch.h"

namespace esphome {
namespace floorheating_switch {

static const char *TAG = "floorheating_switch.switch";

void FloorheatingSwitch::write_state(bool state) {
  if (!this->parent->set_output(zone_, state)) {
        this->publish_state(this->state);
    }

  publish_state(state);
}

void FloorheatingSwitch::dump_config() {
  ESP_LOGCONFIG(TAG, "Floorheating custom switch");
  ESP_LOGCONFIG(TAG, "  zone: %d", zone_);
}

} //namespace floorheating_switch
} //namespace esphome
