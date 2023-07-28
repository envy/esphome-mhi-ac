import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
    CONF_ID,
    CONF_ICON,
)
from . import MhiAc, CONF_MHIAC_ID

DEPENDENCIES = ['mhi_ac']
AUTO_LOAD = ['select']

CONF_VERTICAL_VANES = "vertical_vanes"
CONF_SELECTION_UP = "up"
CONF_SELECTION_UP_DEFAULT = "Up"
CONF_SELECTION_MIDUP = "mid_up"
CONF_SELECTION_MIDUP_DEFAULT = "Mid Up"
CONF_SELECTION_MIDDOWN = "mid_down"
CONF_SELECTION_MIDDOWN_DEFAULT = "Mid Down"
CONF_SELECTION_DOWN = "down"
CONF_SELECTION_DOWN_DEFAULT = "Down"
CONF_SELECTION_SWING = "swing"
CONF_SELECTION_SWING_DEFAULT = "Swing"

TYPES = [
    CONF_VERTICAL_VANES,
]

MhiAcVerticalVanesSelect = cg.global_ns.class_("MhiAcVerticalVanesSelect", select.Select, cg.Component)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_MHIAC_ID): cv.use_id(MhiAc),
            cv.Optional(CONF_VERTICAL_VANES): select.SELECT_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(MhiAcVerticalVanesSelect),
                    cv.Optional(CONF_SELECTION_UP, default=CONF_SELECTION_UP_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_MIDUP, default=CONF_SELECTION_MIDUP_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_MIDDOWN, default=CONF_SELECTION_MIDDOWN_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_DOWN, default=CONF_SELECTION_DOWN_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_SWING, default=CONF_SELECTION_SWING_DEFAULT): cv.string,
                    cv.Optional(CONF_ICON, default="mdi:compare-vertical"): cv.icon,
                }
            ),
        }
    )
)

async def to_code(config):
    mhiAcVar = await cg.get_variable(config[CONF_MHIAC_ID])
    for key in TYPES:
        if key in config:
            conf = config[key]
            if "icon" not in conf:
                conf["conf"] = "mdi:compare-vertical"
            mhiAcSelectVar = cg.new_Pvariable(conf[CONF_ID])
            options = [conf[CONF_SELECTION_UP], conf[CONF_SELECTION_MIDUP], conf[CONF_SELECTION_MIDDOWN], conf[CONF_SELECTION_DOWN], conf[CONF_SELECTION_SWING]]
            await select.register_select(mhiAcSelectVar, conf, options=options)
            cg.add(mhiAcSelectVar.set_mhi_ac(mhiAcVar))
            cg.add(getattr(mhiAcVar, f"set_{key}_select_cb")(mhiAcSelectVar))
