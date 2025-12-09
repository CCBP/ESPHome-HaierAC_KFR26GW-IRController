import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, switch, select
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_PIN,
    CONF_INVERTED,
    CONF_CUSTOM_PRESETS,
    CONF_RESTORE_STATE,
    CONF_OPTIONS,
    CONF_INITIAL_VALUE,
    CONF_STEP,
    PLATFORM_ESP32,
    PLATFORM_ESP8266,
)
from esphome.core import CORE
from esphome import pins

AUTO_LOAD = ["climate", "switch", "select"]

CONF_DISPLAY_SWITCH = "display_switch"
CONF_AUX_HEATING_SWITCH = "aux_heating_switch"
CONF_SWING_MODE_SELECT = "swing_mode_select"
CONF_TIMER_HOUR_SELECT = "timer_hour_select"
CONF_TIMER_MINUTE_SELECT = "timer_minute_select"
CONF_MIN = "min"
CONF_MAX = "max"

haier_ac160_ns = cg.esphome_ns.namespace("haier_ac160")
ClimateIRHaierAC160 = haier_ac160_ns.class_(
    "ClimateIRHaierAC160", climate.Climate, cg.Component)

ClimateIRHaierAC160Switch = haier_ac160_ns.class_(
    "ClimateIRHaierAC160Switch", switch.Switch, cg.Component)
ClimateIRHaierAC160Select = haier_ac160_ns.class_(
    "ClimateIRHaierAC160Select", select.Select, cg.Component)

CustomPresetStr = haier_ac160_ns.namespace("HaierAC160PresetStr")
CUSTOM_PRESETS = {
    "SELF_CLEAN": CustomPresetStr.SELF_CLEAN,
}
validate_custom_presets = cv.enum(CUSTOM_PRESETS, upper=True)

SwingMode = haier_ac160_ns.enum("HaierAC160SwingMode")
SwingModeStr = haier_ac160_ns.class_("ClimateIRHaierAC160SwingModeStr")
SWING_MODE = {
    "Off"       : SwingMode.SWING_OFF,
    "Auto"      : SwingMode.SWING_AUTO,
    "Top"       : SwingMode.SWING_TOP,
    "Highest"   : SwingMode.SWING_HIGHEST,
    "High"      : SwingMode.SWING_HIGH,
    "Middle"    : SwingMode.SWING_MIDDLE,
    "Low"       : SwingMode.SWING_LOW,
    "Lowest"    : SwingMode.SWING_LOWEST,
}

CONFIG_TIMER_HOUR_SCHEMA = {
    cv.Optional(CONF_MIN, default=0): cv.int_range(min=0, max=23),
    cv.Optional(CONF_MAX, default=23): cv.int_range(min=0, max=23),
    cv.Optional(CONF_STEP, default=1): cv.int_range(min=0, max=23),
    cv.Optional(CONF_INITIAL_VALUE, default="--"): cv.string,
    cv.Optional(CONF_OPTIONS):
        cv.invalid(f"Do not set options manually for {CONF_TIMER_HOUR_SELECT}"),
}

CONFIG_TIMER_MINUTE_SCHEMA = {
    cv.Optional(CONF_MIN, default=0): cv.int_range(min=0, max=59),
    cv.Optional(CONF_MAX, default=59): cv.int_range(min=0, max=59),
    cv.Optional(CONF_STEP, default=1): cv.int_range(min=0, max=59),
    cv.Optional(CONF_INITIAL_VALUE, default="--"): cv.string,
    cv.Optional(CONF_OPTIONS):
        cv.invalid(f"Do not set options manually for {CONF_TIMER_MINUTE_SELECT}"),
}

CONFIG_SCHEMA = cv.All(
    climate.climate_schema(ClimateIRHaierAC160).extend(
        {
            cv.GenerateID(): cv.declare_id(ClimateIRHaierAC160),
            cv.Required(CONF_PIN): pins.internal_gpio_output_pin_number,
            cv.Optional(CONF_RESTORE_STATE, default=True): cv.boolean,
            cv.Optional(CONF_INVERTED, default=False): cv.boolean,
            cv.Optional(
                CONF_CUSTOM_PRESETS,
                default=list(CUSTOM_PRESETS.keys())
            ): cv.ensure_list(validate_custom_presets),
            cv.Optional(
                CONF_DISPLAY_SWITCH,
                default={ CONF_NAME: "Display" }
            ): cv.maybe_simple_value(
                switch.switch_schema(ClimateIRHaierAC160Switch),
                key=CONF_NAME,
            ),
            cv.Optional(
                CONF_AUX_HEATING_SWITCH,
                default={ CONF_NAME: "Auxiliary Heating" }
            ): cv.maybe_simple_value(
                switch.switch_schema(ClimateIRHaierAC160Switch),
                key=CONF_NAME,
            ),
            cv.Optional(
                CONF_SWING_MODE_SELECT,
                default={ CONF_NAME: "Swing Mode" }
            ): cv.maybe_simple_value(
                select.select_schema(
                    ClimateIRHaierAC160Select
                ).extend(
                    {
                        cv.Optional(CONF_OPTIONS):
                            cv.invalid(f"Do not set options manually for {CONF_SWING_MODE_SELECT}"),
                    },
                ),
                key=CONF_NAME,
            ),
            cv.Optional(
                CONF_TIMER_HOUR_SELECT,
                default={ CONF_NAME: "Hour" }
            ): cv.maybe_simple_value(
                select.select_schema(
                    ClimateIRHaierAC160Select
                ).extend(CONFIG_TIMER_HOUR_SCHEMA),
                key=CONF_NAME,
            ),
            cv.Optional(
                CONF_TIMER_MINUTE_SELECT,
                default={ CONF_NAME: "Minute" }
            ): cv.maybe_simple_value(
                select.select_schema(
                    ClimateIRHaierAC160Select
                ).extend(CONFIG_TIMER_MINUTE_SCHEMA),
                key=CONF_NAME,
            ),
        }
    ),
    cv.only_on([PLATFORM_ESP32, PLATFORM_ESP8266]),
)

def generate_timer_options(config):
    min_val = config.get(CONF_MIN, 0)
    max_val = config.get(CONF_MAX, 23 if CONF_TIMER_HOUR_SELECT in config else 59)
    step = config.get(CONF_STEP, 1)

    options = ["--"]
    current = min_val
    while current <= max_val:
        options.append(f"{current:0>2}")
        current += step

    if not options:
        raise cv.invalid("No options generated. Check min, max and step values.")

    return options

async def to_code(config):
    cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.6")

    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)

    if CONF_CUSTOM_PRESETS in config:
        cg.add(var.set_custom_presets(config[CONF_CUSTOM_PRESETS]))

    display_sw = await switch.new_switch(config[CONF_DISPLAY_SWITCH])
    await cg.register_component(
        display_sw, config[CONF_DISPLAY_SWITCH]
    )
    cg.add(var.set_display_switch(display_sw))

    aux_heating_sw = await switch.new_switch(config[CONF_AUX_HEATING_SWITCH])
    await cg.register_component(
        aux_heating_sw, config[CONF_AUX_HEATING_SWITCH]
    )
    cg.add(var.set_aux_heating_switch(aux_heating_sw))

    swing_mode_select = await select.new_select(
        config[CONF_SWING_MODE_SELECT],
        cg.TemplateArguments(SwingMode),
        options=list(SWING_MODE.keys()),
    )
    await cg.register_component(
        swing_mode_select, config[CONF_SWING_MODE_SELECT]
    )
    cg.add(var.set_swing_mode_select(swing_mode_select))
    
    timer_hour_select = await select.new_select(
        config[CONF_TIMER_HOUR_SELECT],
        cg.TemplateArguments(cg.uint8),
        options=generate_timer_options(config[CONF_TIMER_HOUR_SELECT]),
    )
    await cg.register_component(
        timer_hour_select, config[CONF_TIMER_HOUR_SELECT]
    )
    cg.add(var.set_timer_hour_select(timer_hour_select))
    
    timer_minute_select = await select.new_select(
        config[CONF_TIMER_MINUTE_SELECT],
        cg.TemplateArguments(cg.uint8),
        options=generate_timer_options(config[CONF_TIMER_MINUTE_SELECT]),
    )
    await cg.register_component(
        timer_minute_select, config[CONF_TIMER_MINUTE_SELECT]
    )
    cg.add(var.set_timer_minute_select(timer_minute_select))
    
    cg.add(var.init(config[CONF_PIN],
        config[CONF_RESTORE_STATE], config[CONF_INVERTED]))
