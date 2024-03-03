import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.config_validation import only_with_arduino

AUTO_LOAD = ['climate']
MULTI_CONF = False
ESP_PLATFORMS: ['esp8266', 'esp32']

CONF_MHIAC_ID = 'mhi_ac_id'

CONF_PROCESSING_TIME = 'processing_time'
CONF_PROCESSING_TIME_DEFAULT = 100

CONF_USE_EXTENDED_FRAME_FORMAT = 'use_extended_frame_format'

MhiAc = cg.global_ns.class_('MhiAc', cg.Component)

def validate_extended_frame_format(key):
    def validator_(config):
        # print(config)
        thing = config.get(key)
        eff = True  # TODO: figure out how to find out if this is set
        if thing is not None and not eff:
            raise cv.Invalid(f"'{key}' requires 'use_extended_frame_format: True'")
        return config
    return validator_

CONFIG_SCHEMA = cv.All(
    cv.Schema({
        cv.GenerateID(): cv.declare_id(MhiAc),
        cv.Optional(CONF_PROCESSING_TIME, default=CONF_PROCESSING_TIME_DEFAULT): cv.int_range(min=25, max=500),
        cv.Optional(CONF_USE_EXTENDED_FRAME_FORMAT, default=False): cv.boolean,
    }).extend(cv.COMPONENT_SCHEMA),
    only_with_arduino,
)

async def to_code(config):
    mhiAc = cg.new_Pvariable(config[CONF_ID])
    cg.add(mhiAc.set_processing_time(config[CONF_PROCESSING_TIME]))
    cg.add(mhiAc.set_use_extended_frame_format(config[CONF_USE_EXTENDED_FRAME_FORMAT]))
    await cg.register_component(mhiAc, config)
