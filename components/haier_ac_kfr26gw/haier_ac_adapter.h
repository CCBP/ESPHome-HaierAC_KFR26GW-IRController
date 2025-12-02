#pragma once

#include "esphome/components/climate/climate.h"

namespace esphome {
namespace haier_ac160 {

enum HaierAC160Preset : uint8_t {
    PRESET_NONE,
    PRESET_SLEEP,
    PRESET_COMFORT,
    PRESET_BOOST,
    PRESET_SELF_CLEAN,
};

class Constants {
    public:
        static const char *const SELF_CLEAN;
};

class Converters {
    public:
        static HaierAC160Preset to_haier_preset(climate::ClimatePreset preset);
        static HaierAC160Preset to_haier_preset(const char *preset);
};

} // namespace haier_ac160
} // namespace esphome
