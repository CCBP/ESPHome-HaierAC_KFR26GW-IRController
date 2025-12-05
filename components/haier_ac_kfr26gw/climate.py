import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import (
    CONF_ID,
    CONF_PIN,
    CONF_INVERTED,
    CONF_CUSTOM_PRESETS,
    CONF_LIGHT,
    CONF_HEATER,
    CONF_RESTORE_STATE,
    PLATFORM_ESP32,
    PLATFORM_ESP8266,
)
from esphome.core import CORE
from esphome import pins

AUTO_LOAD = ["climate"]

haier_ac160_ns = cg.esphome_ns.namespace("haier_ac160")
ClimateIRHaierAC160 = haier_ac160_ns.class_(
        "ClimateIRHaierAC160", climate.Climate)
CustomPresetStr = haier_ac160_ns.namespace("HaierAC160PresetStr")

CUSTOM_PRESETS = {
    "SELF_CLEAN": CustomPresetStr.SELF_CLEAN,
}
validate_custom_presets = cv.enum(CUSTOM_PRESETS, upper=True)

CONFIG_SCHEMA = cv.All(
    climate.climate_schema(ClimateIRHaierAC160).extend(
        {
            cv.GenerateID(): cv.declare_id(ClimateIRHaierAC160),
            cv.Required(CONF_PIN): pins.internal_gpio_output_pin_number,
            cv.Optional(CONF_RESTORE_STATE, default=True): cv.boolean,
            cv.Optional(CONF_INVERTED, default=False): cv.boolean,
            cv.Optional(CONF_CUSTOM_PRESETS): cv.ensure_list(validate_custom_presets),
        }
    ),
    cv.only_on([PLATFORM_ESP32, PLATFORM_ESP8266]),
)

async def to_code(config):
    cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.6")

    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)
    
    cg.add(var.init(config[CONF_PIN],
        config[CONF_RESTORE_STATE], config[CONF_INVERTED]))

    if CONF_CUSTOM_PRESETS in config:
        cg.add(var.set_custom_presets(config[CONF_CUSTOM_PRESETS]))
