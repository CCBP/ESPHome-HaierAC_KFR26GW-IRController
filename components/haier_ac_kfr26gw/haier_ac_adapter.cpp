#include "haier_ac_adapter.h"

namespace esphome {
namespace haier_ac160 {

const char *const HaierAC160PresetStr::SELF_CLEAN       = "自清洁";

HaierAC160Preset Converters::to_haier_preset(climate::ClimatePreset preset) {
    switch (preset) {
        case climate::CLIMATE_PRESET_SLEEP:
            return HaierAC160Preset::PRESET_SLEEP;
        case climate::CLIMATE_PRESET_COMFORT:
            return HaierAC160Preset::PRESET_COMFORT;
        case climate::CLIMATE_PRESET_BOOST:
            return HaierAC160Preset::PRESET_BOOST;
        default:
            return HaierAC160Preset::PRESET_NONE;
    }
}

HaierAC160Preset Converters::to_haier_preset(const char *preset) {
    if (strcmp(preset, HaierAC160PresetStr::SELF_CLEAN) == 0)
        return HaierAC160Preset::PRESET_SELF_CLEAN;
    return HaierAC160Preset::PRESET_NONE;
}

} // namespace haier_ac160
} // namespace esphome
