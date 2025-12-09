#pragma once

#include <iterator>

namespace esphome {
namespace haier_ac160 {

struct StringToUint8 {
    const char *str;
    const uint8_t value;
};

enum HaierAC160Preset : uint8_t {
    PRESET_NONE,
    PRESET_SLEEP,
    PRESET_COMFORT,
    PRESET_BOOST,
    PRESET_SELF_CLEAN,
};

enum HaierAC160SwingMode : uint8_t {
    SWING_OFF,
    SWING_AUTO,
    SWING_TOP,
    SWING_HIGHEST,
    SWING_HIGH,
    SWING_MIDDLE,
    SWING_LOW,
    SWING_LOWEST,
};

constexpr const char *SWING_MODE_STR[] {
    [SWING_OFF] = "OFF", 
    [SWING_AUTO] = "AUTO",
    [SWING_TOP] = "TOP",
    [SWING_HIGHEST] = "HIGHEST",
    [SWING_HIGH] = "HIGH",
    [SWING_MIDDLE] = "MIDDLE",
    [SWING_LOW] = "LOW",
    [SWING_LOWEST] = "LOWEST",
};
#define get_swing_mode_str(swing_mode) ( \
    (swing_mode) < std::size(SWING_MODE_STR) ? \
        SWING_MODE_STR[swing_mode] : "UNKNOW" \
)

class HaierAC160PresetStr {
    public:
        static const char *const SELF_CLEAN;
};

} // namespace haier_ac160
} // namespace esphome
