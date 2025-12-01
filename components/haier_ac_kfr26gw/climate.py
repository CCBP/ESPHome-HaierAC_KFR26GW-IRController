import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID, CONF_PIN, CONF_INVERTED
from esphome.core import CORE
from esphome import pins

AUTO_LOAD = ["climate"]

haier_ac160_ns = cg.esphome_ns.namespace("haier_ac_kfr26gw")
ClimateIRHaierAC160 = haier_ac160_ns.class_(
        "ClimateIRHaierAC160", climate.Climate)

CONFIG_SCHEMA = climate.climate_schema(ClimateIRHaierAC160).extend(
    {
        cv.GenerateID(): cv.declare_id(ClimateIRHaierAC160),
        cv.Required(CONF_PIN): pins.internal_gpio_output_pin_number,
        cv.Optional(CONF_INVERTED, default=False): cv.boolean,
    }
)

async def to_code(config):
    if CORE.is_esp8266 or CORE.is_esp32:
        cg.add_library("crankyoldgit/IRremoteESP8266", "2.8.6")

    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)
    
    cg.add(var.init(config[CONF_PIN], config[CONF_INVERTED]))
