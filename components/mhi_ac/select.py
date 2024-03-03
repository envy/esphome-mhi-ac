import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
    CONF_ID,
    CONF_ICON,
)
from . import MhiAc, CONF_MHIAC_ID, validate_extended_frame_format

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

CONF_HORIZONTAL_VANES = "horizontal_vanes"
CONF_SELECTION_LEFT = "left"  # 1
CONF_SELECTION_LEFT_DEFAULT = "Left"
CONF_SELECTION_MIDLEFT = "mid_left"  # 2
CONF_SELECTION_MIDLEFT_DEFAULT = "Mid Left"
CONF_SELECTION_MIDDLE = "middle"  # 3
CONF_SELECTION_MIDDLE_DEFAULT = "Middle"
CONF_SELECTION_MIDRIGHT = "mid_right"  # 4
CONF_SELECTION_MIDRIGHT_DEFAULT = "Mid Right"
CONF_SELECTION_RIGHT = "right"  # 4
CONF_SELECTION_RIGHT_DEFAULT = "Right"
CONF_SELECTION_OUTER = "outer"  # 6
CONF_SELECTION_OUTER_DEFAULT = "Outer"
CONF_SELECTION_INNER = "inner"  # 7
CONF_SELECTION_INNER_DEFAULT = "Inner"

TYPES = [
    CONF_VERTICAL_VANES,
    CONF_HORIZONTAL_VANES,
]

MhiAcVerticalVanesSelect = cg.global_ns.class_("MhiAcVerticalVanesSelect", select.Select, cg.Component)
MhiAcHorizontalVanesSelect = cg.global_ns.class_("MhiAcHorizontalVanesSelect", select.Select, cg.Component)

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
            cv.Optional(CONF_HORIZONTAL_VANES): select.SELECT_SCHEMA.extend(
                {
                    cv.GenerateID(): cv.declare_id(MhiAcHorizontalVanesSelect),
                    cv.Optional(CONF_SELECTION_LEFT, default=CONF_SELECTION_LEFT_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_MIDLEFT, default=CONF_SELECTION_MIDLEFT_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_MIDDLE, default=CONF_SELECTION_MIDDLE_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_MIDRIGHT, default=CONF_SELECTION_MIDRIGHT_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_RIGHT, default=CONF_SELECTION_RIGHT_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_OUTER, default=CONF_SELECTION_OUTER_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_INNER, default=CONF_SELECTION_INNER_DEFAULT): cv.string,
                    cv.Optional(CONF_SELECTION_SWING, default=CONF_SELECTION_SWING_DEFAULT): cv.string,
                    cv.Optional(CONF_ICON, default="mdi:compare-horizontal"): cv.icon,
                }
            ),
        }
    ),
    validate_extended_frame_format(CONF_HORIZONTAL_VANES)
)

async def to_code(config):
    mhiAcVar = await cg.get_variable(config[CONF_MHIAC_ID])
    for key in TYPES:
        if key in config:
            conf = config[key]
            mhiAcSelectVar = cg.new_Pvariable(conf[CONF_ID])
            options = []
            if key == CONF_VERTICAL_VANES:
                options = [
                    conf[CONF_SELECTION_UP],
                    conf[CONF_SELECTION_MIDUP],
                    conf[CONF_SELECTION_MIDDOWN],
                    conf[CONF_SELECTION_DOWN],
                    conf[CONF_SELECTION_SWING]
                ]
            elif key == CONF_HORIZONTAL_VANES:
                options = [
                    conf[CONF_SELECTION_LEFT],
                    conf[CONF_SELECTION_MIDLEFT],
                    conf[CONF_SELECTION_MIDDLE],
                    conf[CONF_SELECTION_MIDRIGHT],
                    conf[CONF_SELECTION_RIGHT],
                    conf[CONF_SELECTION_OUTER],
                    conf[CONF_SELECTION_INNER],
                    conf[CONF_SELECTION_SWING]
                ]
            await select.register_select(mhiAcSelectVar, conf, options=options)
            cg.add(mhiAcSelectVar.set_mhi_ac(mhiAcVar))
            cg.add(getattr(mhiAcVar, f"set_{key}_select_cb")(mhiAcSelectVar))
