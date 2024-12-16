import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart, binary_sensor
from esphome.const import *

floorheating_ns = cg.esphome_ns.namespace('floorheatingcomponent')
FloorheatingComponent = floorheating_ns.class_("FloorheatingComponent", cg.PollingComponent)

DEPENDENCIES = ['uart']
AUTO_LOAD = ['uart', 'sensor', "binary_sensor" ]

CONF_TEMP_ZONE_1 = "temp_zone_1"
CONF_TEMP_ZONE_2 = "temp_zone_2"
CONF_TEMP_ZONE_3 = "temp_zone_3"
CONF_TEMP_ZONE_4 = "temp_zone_4"
CONF_TEMP_ZONE_5 = "temp_zone_5"
CONF_TEMP_ZONE_6 = "temp_zone_6"
CONF_TEMP_ZONE_7 = "temp_zone_7"
CONF_TEMP_ZONE_8 = "temp_zone_8"
CONF_TEMP_ZONE_9 = "temp_zone_9"
CONF_TEMP_ZONE_10 = "temp_zone_10"
CONF_TEMP_ZONE_11 = "temp_zone_11"
CONF_TEMP_ZONE_12 = "temp_zone_12"

CONF_SWITCH_ZONE_1 = "switch_zone_1"
CONF_SWITCH_ZONE_2 = "switch_zone_2"
CONF_SWITCH_ZONE_3 = "switch_zone_3"
CONF_SWITCH_ZONE_4 = "switch_zone_4"
CONF_SWITCH_ZONE_5 = "switch_zone_5"
CONF_SWITCH_ZONE_6 = "switch_zone_6"
CONF_SWITCH_ZONE_7 = "switch_zone_7"
CONF_SWITCH_ZONE_8 = "switch_zone_8"
CONF_SWITCH_ZONE_9 = "switch_zone_9"
CONF_SWITCH_ZONE_10 = "switch_zone_10"
CONF_SWITCH_ZONE_11 = "switch_zone_11"
CONF_SWITCH_ZONE_12 = "switch_zone_12"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(FloorheatingComponent),    

    cv.Optional(CONF_TEMP_ZONE_1):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_2):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_3):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_4):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_5):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_6):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_7):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_8):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_9):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_10):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_11):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_TEMP_ZONE_12):
        sensor.sensor_schema(device_class=DEVICE_CLASS_TEMPERATURE,unit_of_measurement=UNIT_CELSIUS,accuracy_decimals=2,state_class=STATE_CLASS_MEASUREMENT).extend(),

    cv.Optional(CONF_SWITCH_ZONE_1):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_2):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_3):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_4):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_5):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_6):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_7):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_8):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_9):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_10):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_11):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
    cv.Optional(CONF_SWITCH_ZONE_12):
        binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_HEAT).extend(),
}).extend(cv.polling_component_schema('60s')).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)

    if CONF_TEMP_ZONE_1 in config:
        conf = config[CONF_TEMP_ZONE_1]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_1_sensor(sens))
    if CONF_TEMP_ZONE_2 in config:
        conf = config[CONF_TEMP_ZONE_2]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_2_sensor(sens))
    if CONF_TEMP_ZONE_3 in config:
        conf = config[CONF_TEMP_ZONE_3]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_3_sensor(sens))
    if CONF_TEMP_ZONE_4 in config:
        conf = config[CONF_TEMP_ZONE_4]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_4_sensor(sens))
    if CONF_TEMP_ZONE_5 in config:
        conf = config[CONF_TEMP_ZONE_5]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_5_sensor(sens))
    if CONF_TEMP_ZONE_6 in config:
        conf = config[CONF_TEMP_ZONE_6]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_6_sensor(sens))
    if CONF_TEMP_ZONE_7 in config:
        conf = config[CONF_TEMP_ZONE_7]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_7_sensor(sens))
    if CONF_TEMP_ZONE_8 in config:
        conf = config[CONF_TEMP_ZONE_8]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_8_sensor(sens))
    if CONF_TEMP_ZONE_9 in config:
        conf = config[CONF_TEMP_ZONE_9]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_9_sensor(sens))
    if CONF_TEMP_ZONE_10 in config:
        conf = config[CONF_TEMP_ZONE_10]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_10_sensor(sens))
    if CONF_TEMP_ZONE_11 in config:
        conf = config[CONF_TEMP_ZONE_11]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_11_sensor(sens))
    if CONF_TEMP_ZONE_12 in config:
        conf = config[CONF_TEMP_ZONE_12]
        sens = yield sensor.new_sensor(conf)
        cg.add(var.set_temp_zone_12_sensor(sens))

    if CONF_SWITCH_ZONE_1 in config:
        conf = config[CONF_SWITCH_ZONE_1]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_1_sensor(sens))
        # cg.add(var.set_internal(sens))
    if CONF_SWITCH_ZONE_2 in config:
        conf = config[CONF_SWITCH_ZONE_2]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_2_sensor(sens))
    if CONF_SWITCH_ZONE_3 in config:
        conf = config[CONF_SWITCH_ZONE_3]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_3_sensor(sens))
    if CONF_SWITCH_ZONE_4 in config:
        conf = config[CONF_SWITCH_ZONE_4]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_4_sensor(sens))
    if CONF_SWITCH_ZONE_5 in config:
        conf = config[CONF_SWITCH_ZONE_5]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_5_sensor(sens))
    if CONF_SWITCH_ZONE_6 in config:
        conf = config[CONF_SWITCH_ZONE_6]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_6_sensor(sens))
    if CONF_SWITCH_ZONE_7 in config:
        conf = config[CONF_SWITCH_ZONE_7]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_7_sensor(sens))
    if CONF_SWITCH_ZONE_8 in config:
        conf = config[CONF_SWITCH_ZONE_8]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_8_sensor(sens))
    if CONF_SWITCH_ZONE_9 in config:
        conf = config[CONF_SWITCH_ZONE_9]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_9_sensor(sens))
    if CONF_SWITCH_ZONE_10 in config:
        conf = config[CONF_SWITCH_ZONE_10]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_10_sensor(sens))
    if CONF_SWITCH_ZONE_11 in config:
        conf = config[CONF_SWITCH_ZONE_11]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_11_sensor(sens))
    if CONF_SWITCH_ZONE_12 in config:
        conf = config[CONF_SWITCH_ZONE_12]
        sens = yield binary_sensor.new_binary_sensor(conf)
        cg.add(var.set_switch_zone_12_sensor(sens))                
