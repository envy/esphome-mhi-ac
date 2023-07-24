import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.core import coroutine
from esphome.const import CONF_ID
from . import MhiAc, CONF_MHIAC_ID

DEPENDENCIES = ['mhi_ac']
AUTO_LOAD = ['climate']

DEFAULT_CLIMATE_MODES = ['COOL', 'HEAT', 'DRY', 'FAN_ONLY', 'AUTO']
DEFAULT_FAN_MODES = ['QUIET', 'LOW', 'MEDIUM', 'MIDDLE', 'HIGH', 'AUTO']
DEFAULT_SWING_MODES = ['OFF', 'VERTICAL']

MhiAcClimate = cg.global_ns.class_("MhiAcClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(MhiAcClimate),
        cv.GenerateID(CONF_MHIAC_ID): cv.use_id(MhiAc)
    }
).extend(cv.COMPONENT_SCHEMA)

@coroutine
def to_code(config):
    mhiAcClimateVar = cg.new_Pvariable(config[CONF_ID])
    mhiAcVar = yield cg.get_variable(config[CONF_MHIAC_ID])
    yield climate.register_climate(mhiAcClimateVar, config)
    yield cg.add(mhiAcVar.set_climate_cb(mhiAcClimateVar))
    yield cg.add(mhiAcClimateVar.set_mhi_ac(mhiAcVar))
