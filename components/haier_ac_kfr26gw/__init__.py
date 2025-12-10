import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number, switch, select
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_PIN,
    CONF_INVERTED,
    CONF_RESTORE_STATE,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
    CONF_OPTIONS,
    CONF_INITIAL_VALUE,
    PLATFORM_ESP32,
    PLATFORM_ESP8266,
)
from esphome.core import CORE
from esphome import pins

AUTO_LOAD = ["number", "switch", "select"]

CONF_TEMPERATURE_NUMBER = "temperature_number"
CONF_POWER_SWITCH = "power_switch"
CONF_SLEEP_SWITCH = "sleep_switch"
CONF_LOCK_SWITCH = "lock_switch"
CONF_DISPLAY_SWITCH = "display_switch"
CONF_AUX_HEATING_SWITCH = "aux_heating_switch"
CONF_SWING_MODE_SELECT = "swing_mode_select"
CONF_OPERATE_MODE_SELECT = "operate_mode_select"
CONF_FAN_SPEED_SELECT = "fan_speed_select"
CONF_TIMER_HOUR_SELECT = "timer_hour_select"
CONF_TIMER_MINUTE_SELECT = "timer_minute_select"

haier_ac160_ns = cg.esphome_ns.namespace("haier_ac160")
HaierAC160 = haier_ac160_ns.class_("HaierAC160", cg.Component)

HaierAC160Number = haier_ac160_ns.class_(
    "HaierAC160Number", number.Number, cg.Component)
HaierAC160Switch = haier_ac160_ns.class_(
    "HaierAC160Switch", switch.Switch, cg.Component)
HaierAC160Select = haier_ac160_ns.class_(
    "HaierAC160Select", select.Select, cg.Component)

OperateMode = haier_ac160_ns.enum("HaierAC160OperateMode")
OPERATE_MODE = {
    "MODE_AUTO" : OperateMode.MODE_AUTO,
    "MODE_COOL" : OperateMode.MODE_COOL,
    "MODE_HEAT" : OperateMode.MODE_HEAT,
    "MODE_DRY"  : OperateMode.MODE_DRY,
    "MODE_FAN"  : OperateMode.MODE_FAN,
}

SwingMode = haier_ac160_ns.enum("HaierAC160SwingMode")
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

FanSpeed = haier_ac160_ns.enum("HaierAC160FanSpeed")
FAN_SPEED = {
    "SPEED_AUTO"    : FanSpeed.SPEED_AUTO,
    "SPEED_LOW"     : FanSpeed.SPEED_LOW,
    "SPEED_MEDIUM"  : FanSpeed.SPEED_MEDIUM,
    "SPEED_HIGH"    : FanSpeed.SPEED_HIGH,
}

def select_options_invalid(cfg_name: str):
    return {
        cv.Optional(CONF_OPTIONS):
            cv.invalid(f"Do not set options manually for {cfg_name}"),
    }

CONFIG_TIMER_HOUR_SCHEMA = {
    cv.Optional(CONF_MIN_VALUE, default=0): cv.int_range(min=0, max=23),
    cv.Optional(CONF_MAX_VALUE, default=23): cv.int_range(min=0, max=23),
    cv.Optional(CONF_STEP, default=1): cv.int_range(min=0, max=23),
    cv.Optional(CONF_INITIAL_VALUE, default="--"): cv.string,
}

CONFIG_TIMER_MINUTE_SCHEMA = {
    cv.Optional(CONF_MIN_VALUE, default=0): cv.int_range(min=0, max=59),
    cv.Optional(CONF_MAX_VALUE, default=59): cv.int_range(min=0, max=59),
    cv.Optional(CONF_STEP, default=1): cv.int_range(min=0, max=59),
    cv.Optional(CONF_INITIAL_VALUE, default="--"): cv.string,
}

CONFIG_SCHEMA = cv.All(
    {
        cv.GenerateID(): cv.declare_id(HaierAC160),
        cv.Optional(CONF_NAME): cv.string,
        cv.Required(CONF_PIN): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_RESTORE_STATE, default=True): cv.boolean,
        cv.Optional(CONF_INVERTED, default=False): cv.boolean,
        cv.Optional(
            CONF_TEMPERATURE_NUMBER,
            default={ CONF_NAME: "Temperature" }
        ): cv.maybe_simple_value(
            number.number_schema(HaierAC160Number)
            .extend(cv.COMPONENT_SCHEMA)
            .extend(
                {
                    cv.Optional(CONF_MIN_VALUE, default=16):
                        cv.int_range(min=16, max=30),
                    cv.Optional(CONF_MAX_VALUE, default=30):
                        cv.int_range(min=16, max=30),
                    cv.Optional(CONF_INITIAL_VALUE, default=25):
                        cv.int_range(min=16, max=30),
                    cv.Optional(CONF_STEP, default=1): cv.int_,
                }
            ),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_POWER_SWITCH,
            default={ CONF_NAME: "Power" }
        ): cv.maybe_simple_value(
            switch.switch_schema(HaierAC160Switch),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_SLEEP_SWITCH,
            default={ CONF_NAME: "Sleep" }
        ): cv.maybe_simple_value(
            switch.switch_schema(HaierAC160Switch),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_LOCK_SWITCH,
            default={ CONF_NAME: "Lock" }
        ): cv.maybe_simple_value(
            switch.switch_schema(HaierAC160Switch),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_DISPLAY_SWITCH,
            default={ CONF_NAME: "Display" }
        ): cv.maybe_simple_value(
            switch.switch_schema(HaierAC160Switch),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_AUX_HEATING_SWITCH,
            default={ CONF_NAME: "Auxiliary Heating" }
        ): cv.maybe_simple_value(
            switch.switch_schema(HaierAC160Switch),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_OPERATE_MODE_SELECT,
            default={ CONF_NAME: "Operate Mode" }
        ): cv.maybe_simple_value(
            select.select_schema(HaierAC160Select)
            .extend(select_options_invalid(CONF_OPERATE_MODE_SELECT)),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_SWING_MODE_SELECT,
            default={ CONF_NAME: "Swing Mode" }
        ): cv.maybe_simple_value(
            select.select_schema(HaierAC160Select)
            .extend(select_options_invalid(CONF_SWING_MODE_SELECT)),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_FAN_SPEED_SELECT,
            default={ CONF_NAME: "Fan Speed" }
        ): cv.maybe_simple_value(
            select.select_schema(HaierAC160Select)
            .extend(select_options_invalid(CONF_FAN_SPEED_SELECT)),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_TIMER_HOUR_SELECT,
            default={ CONF_NAME: "Hour" }
        ): cv.maybe_simple_value(
            select.select_schema(HaierAC160Select)
            .extend(CONFIG_TIMER_HOUR_SCHEMA)
            .extend(select_options_invalid(CONF_FAN_SPEED_SELECT)),
            key=CONF_NAME,
        ),
        cv.Optional(
            CONF_TIMER_MINUTE_SELECT,
            default={ CONF_NAME: "Minute" }
        ): cv.maybe_simple_value(
            select.select_schema(HaierAC160Select)
            .extend(CONFIG_TIMER_MINUTE_SCHEMA)
            .extend(select_options_invalid(CONF_FAN_SPEED_SELECT)),
            key=CONF_NAME,
        ),
    },
    cv.only_on([PLATFORM_ESP32, PLATFORM_ESP8266]),
)

def generate_timer_options(config):
    min_val = config.get(CONF_MIN_VALUE, 0)
    max_val = config.get(CONF_MAX_VALUE,
        23 if CONF_TIMER_HOUR_SELECT in config else 59)
    step = config.get(CONF_STEP, 1)

    options = ["--"]
    current = min_val
    while current <= max_val:
        options.append(f"{current:0>2}")
        current += step

    if not options:
        raise cv.invalid("No options generated. "
            "Check min, max and step values.")

    return options

async def to_code(config):
    cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.6")

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    var.set_timer_hour_step(config[CONF_TIMER_HOUR_SELECT][CONF_STEP])
    var.set_timer_minute_step(config[CONF_TIMER_MINUTE_SELECT][CONF_STEP])

    for conf, set_func in [
        ( CONF_TEMPERATURE_NUMBER, var.set_temperature_number ),
    ]:
        nu = await number.new_number(
            config[conf],
            min_value=config[conf][CONF_MIN_VALUE],
            max_value=config[conf][CONF_MAX_VALUE],
            step=config[conf][CONF_STEP],
        )
        await cg.register_component(nu, config[conf])
        cg.add(set_func(nu))

    for conf, set_func in [
        ( CONF_POWER_SWITCH, var.set_power_switch ),
        ( CONF_SLEEP_SWITCH, var.set_sleep_switch ),
        ( CONF_LOCK_SWITCH, var.set_lock_switch ),
        ( CONF_DISPLAY_SWITCH, var.set_display_switch ),
        ( CONF_AUX_HEATING_SWITCH, var.set_aux_heating_switch ),
    ]:
        sw = await switch.new_switch(config[conf])
        await cg.register_component(sw, config[conf])
        cg.add(set_func(sw))

    for conf, arg_type, opts, set_func in [
        (
            CONF_OPERATE_MODE_SELECT, OperateMode,
            list(OPERATE_MODE.keys()), var.set_operate_mode_select
        ),
        (
            CONF_SWING_MODE_SELECT, SwingMode,
            list(SWING_MODE.keys()), var.set_swing_mode_select
        ),
        (
            CONF_FAN_SPEED_SELECT, FanSpeed,
            list(FAN_SPEED.keys()), var.set_fan_speed_select
        ),
        (
            CONF_TIMER_HOUR_SELECT, cg.uint8,
            generate_timer_options(config[CONF_TIMER_HOUR_SELECT]),
            var.set_timer_hour_select
        ),
        (
            CONF_TIMER_MINUTE_SELECT, cg.uint8,
            generate_timer_options(config[CONF_TIMER_MINUTE_SELECT]),
            var.set_timer_minute_select
        ),
    ]:
        se = await select.new_select(
            config[conf],
            cg.TemplateArguments(arg_type),
            options=opts,
        )
        await cg.register_component(se, config[conf])
        cg.add(set_func(se))
    
    cg.add(var.init(config[CONF_PIN],
        config[CONF_RESTORE_STATE], config[CONF_INVERTED]))
