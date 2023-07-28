import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.core import coroutine

AUTO_LOAD = ['climate']
MULTI_CONF = False

CONF_MHIAC_ID = 'mhi_ac_id'

CONF_PROCESSING_TIME = 'processing_time'
CONF_PROCESSING_TIME_DEFAULT = 100

MhiAc = cg.global_ns.class_('MhiAc', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MhiAc),
    cv.Optional(CONF_PROCESSING_TIME, default=CONF_PROCESSING_TIME_DEFAULT): cv.int_range(min=25, max=500),
}).extend(cv.COMPONENT_SCHEMA)

@coroutine
def to_code(config):
    mhiAc = cg.new_Pvariable(config[CONF_ID])
    yield cg.add(mhiAc.set_processing_time(config[CONF_PROCESSING_TIME]))
    yield cg.register_component(mhiAc, config)
