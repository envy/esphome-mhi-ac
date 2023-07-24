import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_RAW,
    CONF_ID,
    CONF_ADDRESS,
    CONF_INPUT,
    CONF_NUMBER,
    CONF_HARDWARE_UART,
    CONF_TEMPERATURE,
    CONF_VOLTAGE,
    CONF_CURRENT,
    CONF_BATTERY_LEVEL,
    
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
    UNIT_VOLT,
    UNIT_CELSIUS,
    UNIT_AMPERE,
    CONF_UPDATE_INTERVAL,
    UNIT_EMPTY,
    UNIT_PERCENT,
    ICON_EMPTY,
    ICON_THERMOMETER,
    ICON_FLASH,
    ICON_PERCENT,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_TEMPERATURE,
)
from . import MhiAc, CONF_MHIAC_ID

DEPENDENCIES = ['mhi_ac']
AUTO_LOAD = ['climate']

CONF_OUTDOOR_TEMPERATURE = "outdoor_temperature"

TYPES = [
    CONF_OUTDOOR_TEMPERATURE,
    # CONF_CURRENT,
]

MhiAcSensor = cg.global_ns.class_("MhiAcSensor", cg.Component)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MhiAcSensor),
            cv.GenerateID(CONF_MHIAC_ID): cv.use_id(MhiAc),
            cv.Optional(CONF_OUTDOOR_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                icon=ICON_THERMOMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            # cv.Optional(CONF_CURRENT): sensor.sensor_schema(
            #     unit_of_measurement=UNIT_AMPERE,
            #     icon="mdi:current-ac",
            #     accuracy_decimals=1,
            #     device_class=DEVICE_CLASS_CURRENT,
            #     state_class=STATE_CLASS_MEASUREMENT,
            # ),
        }
    )
)

async def to_code(config):
    mhiAcSensorVar = cg.new_Pvariable(config[CONF_ID])
    mhiAcVar = await cg.get_variable(config[CONF_MHIAC_ID])
    cg.add(mhiAcVar.set_sensor_cb(mhiAcSensorVar))
    cg.add(mhiAcSensorVar.set_mhi_ac(mhiAcVar))
    for key in TYPES:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(mhiAcSensorVar, f"set_{key}_sensor")(sens))
