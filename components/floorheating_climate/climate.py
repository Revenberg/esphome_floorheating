import voluptuous as vol
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, sensor, switch, floorheating_component
from esphome.const import CONF_ID, CONF_SENSOR

CONF_FH = "floorheating_component"
AUTO_LOAD = ["climate", "sensor", "binary_sensor", "floorheating_component"]

floorheating_climate_ns = cg.esphome_ns.namespace("floorheating_climate")
FloorheatingClimate = floorheating_climate_ns.class_("FloorheatingClimate", climate.Climate, switch.Switch, cg.Component)

CONF_ZONE_1 = "zone_1"
CONF_ZONE_2 = "zone_2"
CONF_ZONE_3 = "zone_3"
CONF_ZONE_4 = "zone_4"
CONF_ZONE_5 = "zone_5"
CONF_ZONE_6 = "zone_6"
CONF_ZONE_7 = "zone_7"
CONF_ZONE_8 = "zone_8"
CONF_ZONE_9 = "zone_9"
CONF_ZONE_10 = "zone_10"
CONF_ZONE_11 = "zone_11"
CONF_ZONE_12 = "zone_12"

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(FloorheatingClimate),   
        cv.Required(CONF_SENSOR): cv.use_id(sensor.Sensor),    
        cv.Required(CONF_FH): cv.use_id(floorheating_component.FloorheatingComponent),    
        cv.Optional(CONF_ZONE_1, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_2, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_3, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_4, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_5, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_6, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_7, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_8, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_9, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_10, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_11, default='false'): cv.boolean,
        cv.Optional(CONF_ZONE_12, default='false'): cv.boolean,
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await climate.register_climate(var, config)
    await cg.register_component(var, config)

    sens = await cg.get_variable(config[CONF_SENSOR])
    cg.add(var.set_sensor(sens))

    fh = await cg.get_variable(config[CONF_FH])
    cg.add(var.set_floorheatingcomponent(fh))

    if CONF_ZONE_1 in config:        
        cg.add(var.set_zones(1, config[CONF_ZONE_1]))
    if CONF_ZONE_2 in config:        
        cg.add(var.set_zones(2, config[CONF_ZONE_2]))
    if CONF_ZONE_3 in config:        
        cg.add(var.set_zones(3, config[CONF_ZONE_3]))
    if CONF_ZONE_4 in config:        
        cg.add(var.set_zones(4, config[CONF_ZONE_4]))
    if CONF_ZONE_5 in config:        
        cg.add(var.set_zones(5, config[CONF_ZONE_5]))
    if CONF_ZONE_6 in config:        
        cg.add(var.set_zones(6, config[CONF_ZONE_6]))
    if CONF_ZONE_7 in config:        
        cg.add(var.set_zones(7, config[CONF_ZONE_7]))
    if CONF_ZONE_8 in config:        
        cg.add(var.set_zones(8, config[CONF_ZONE_8]))
    if CONF_ZONE_9 in config:        
        cg.add(var.set_zones(9, config[CONF_ZONE_9]))
    if CONF_ZONE_10 in config:        
        cg.add(var.set_zones(10, config[CONF_ZONE_10]))
    if CONF_ZONE_11 in config:        
        cg.add(var.set_zones(11, config[CONF_ZONE_11]))
    if CONF_ZONE_12 in config:        
        cg.add(var.set_zones(12, config[CONF_ZONE_12]))
    