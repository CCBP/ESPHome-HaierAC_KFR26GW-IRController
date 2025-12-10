#include "haier_ac_adapter.h"

namespace esphome {
namespace haier_ac160 {

std::map<HaierAC160SwingMode, const char *>
Converts::SWING_MODE_STR = {
    { SWING_OFF, "OFF" },
    { SWING_AUTO, "AUTO" },
    { SWING_TOP, "TOP" },
    { SWING_HIGHEST, "HIGHEST" },
    { SWING_HIGH, "HIGH" },
    { SWING_MIDDLE, "MIDDLE" },
    { SWING_LOW, "LOW" },
    { SWING_LOWEST, "LOWEST" },
};

std::map<HaierAC160FanSpeed, const char *>
Converts::FAN_SPEED_STR = {
    { SPEED_AUTO, "AUTO" },
    { SPEED_LOW, "LOW" },
    { SPEED_MEDIUM, "MEDIUM" },
    { SPEED_HIGH, "HIGH" },
};

const char *const Converts::get_swing_mode_str(HaierAC160SwingMode swing_mode) {
    return get_enum_str(swing_mode, SWING_MODE_STR);
}

const char *const Converts::get_fan_speed_str(HaierAC160FanSpeed fan_speed) {
    return get_enum_str(fan_speed, FAN_SPEED_STR);
}

} // namespace esphome
} // namespace haier_ac160
