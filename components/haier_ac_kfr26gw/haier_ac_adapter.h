#pragma once

#include <iterator>

namespace esphome {
namespace haier_ac160 {

#define get_enum_str(index, enum_name) \
    ((index) < std::size(enum_name)) ? enum_name[index] : "UNKNOW"

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
#define get_swing_mode_str(swing_mode) \
    get_enum_str(swing_mode, SWING_MODE_STR)

enum HaierAC160FanSpeed : uint8_t {
    SPEED_AUTO,
    SPEED_LOW,
    SPEED_MEDIUM,
    SPEED_HIGH,
};

constexpr const char *FAN_SPEED_STR[] {
    [SPEED_AUTO] = "AUTO",
    [SPEED_LOW] = "LOW",
    [SPEED_MEDIUM] = "MEDIUM",
    [SPEED_HIGH] = "HIGH",
};
#define get_fan_speed_str(fan_speed) \
    get_enum_str(fan_speed, FAN_SPEED_STR)

} // namespace haier_ac160
} // namespace esphome
