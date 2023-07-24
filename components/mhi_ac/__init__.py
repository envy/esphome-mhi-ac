import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.core import coroutine

AUTO_LOAD = ['climate']
MULTI_CONF = False

CONF_MHIAC_ID = 'mhi_ac_id'

MhiAc = cg.global_ns.class_('MhiAc', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(MhiAc),
}).extend(cv.COMPONENT_SCHEMA)

@coroutine
def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
