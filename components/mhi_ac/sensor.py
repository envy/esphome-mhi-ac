import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_CURRENT,
    CONF_ENERGY,
    
    ENTITY_CATEGORY_DIAGNOSTIC,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    UNIT_CELSIUS,
    UNIT_AMPERE,
    UNIT_KILOWATT_HOURS,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_TEMPERATURE,
)
from . import MhiAc, CONF_MHIAC_ID

DEPENDENCIES = ['mhi_ac']
AUTO_LOAD = ['climate']

CONF_OUTDOOR_TEMPERATURE = "outdoor_temperature"
CONF_PROTECTION_STATE = "protection_state"

TYPES = [
    CONF_OUTDOOR_TEMPERATURE,
    CONF_CURRENT,
    CONF_ENERGY,
    CONF_PROTECTION_STATE
]

MhiAcSensor = cg.global_ns.class_("MhiAcSensor", cg.Component)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MhiAcSensor),
            cv.GenerateID(CONF_MHIAC_ID): cv.use_id(MhiAc),
            cv.Optional(CONF_OUTDOOR_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_CURRENT): sensor.sensor_schema(
                unit_of_measurement=UNIT_AMPERE,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_CURRENT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ENERGY): sensor.sensor_schema(
                unit_of_measurement=UNIT_KILOWATT_HOURS,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_ENERGY,
                state_class=STATE_CLASS_TOTAL_INCREASING,
            ),
            cv.Optional(CONF_PROTECTION_STATE): sensor.sensor_schema(
                entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
            ),
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
