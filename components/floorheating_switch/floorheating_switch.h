#pragma once

#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../floorheating_component/floorheating_component.h"

namespace esphome {
namespace floorheating_switch {

class FloorheatingSwitch : public switch_::Switch, public Component {
 public:
  void write_state(bool state) override;
  void dump_config() override;
  
  float get_setup_priority() const { return setup_priority::HARDWARE; }
  void set_zone(uint16_t zone) { zone_ = zone; }  

 protected:
  uint16_t zone_{0};

  floorheatingcomponent::FloorheatingComponent* parent;
};

} //namespace floorheating_switch
} //namespace esphome
