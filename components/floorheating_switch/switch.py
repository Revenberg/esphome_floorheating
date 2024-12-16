import voluptuous as vol

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import CONF_OUTPUT

floorheating_switch_ns = cg.esphome_ns.namespace('floorheating_switch')
FloorheatingSwitch = floorheating_switch_ns.class_('FloorheatingSwitch', switch.Switch, cg.Component)

DEPENDENCIES = []
AUTO_LOAD = ['switch']

CONF_ZONE = "zone"

CONFIG_SCHEMA = switch.SWITCH_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(FloorheatingSwitch),
    cv.Required(CONF_ZONE): vol.All(cv.uint16_t, vol.Range(min=0, max=15)),    
    
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    cg.add(var.set_zone(config[CONF_ZONE]))