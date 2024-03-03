import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID
from . import MhiAc, CONF_MHIAC_ID

DEPENDENCIES = ['mhi_ac']
AUTO_LOAD = ['climate']

DEFAULT_CLIMATE_MODES = ['COOL', 'HEAT', 'DRY', 'FAN_ONLY', 'AUTO']
DEFAULT_FAN_MODES = ['QUIET', 'LOW', 'MEDIUM', 'MIDDLE', 'HIGH', 'AUTO']
DEFAULT_SWING_MODES = ['OFF', 'VERTICAL', 'HORIZONTAL', 'BOTH']

MhiAcClimate = cg.global_ns.class_('MhiAcClimate', climate.Climate, cg.Component)

CONF_VERTICAL_SWING_OFF_POSITION = 'vertical_swing_off_positon'
CONF_VERTICAL_SWING_OFF_POSITION_DEFAULT = 2
CONF_HORIZONTAL_SWING_OFF_POSITION = 'horizontal_swing_off_positon'
CONF_HORIZONTAL_SWING_OFF_POSITION_DEFAULT = 3

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(MhiAcClimate),
        cv.GenerateID(CONF_MHIAC_ID): cv.use_id(MhiAc),
        cv.Optional(CONF_VERTICAL_SWING_OFF_POSITION, default=CONF_VERTICAL_SWING_OFF_POSITION_DEFAULT): cv.int_range(min=1, max=4),
        cv.Optional(CONF_HORIZONTAL_SWING_OFF_POSITION, default=CONF_HORIZONTAL_SWING_OFF_POSITION_DEFAULT): cv.int_range(min=1, max=7)
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    mhiAcClimateVar = cg.new_Pvariable(config[CONF_ID])
    mhiAcVar = await cg.get_variable(config[CONF_MHIAC_ID])
    await climate.register_climate(mhiAcClimateVar, config)
    cg.add(mhiAcVar.set_climate_cb(mhiAcClimateVar))
    cg.add(mhiAcClimateVar.set_mhi_ac(mhiAcVar))
    cg.add(mhiAcClimateVar.set_vertical_swing_off_position(config[CONF_VERTICAL_SWING_OFF_POSITION]))
    cg.add(mhiAcClimateVar.set_horizontal_swing_off_position(config[CONF_HORIZONTAL_SWING_OFF_POSITION]))
