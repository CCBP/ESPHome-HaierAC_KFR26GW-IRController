#include "haier_ac_adapter.h"

namespace esphome {
namespace haier_ac160 {

std::map<HaierAC160OperateMode, std::string>
Converts::OPERATE_MODE_STR = {
    { MODE_AUTO, "Auto" },
    { MODE_COOL, "Cool" },
    { MODE_HEAT, "Heat" },
    { MODE_DRY, "Dry" },
    { MODE_FAN, "Fan" },
};

std::map<HaierAC160SwingMode, std::string>
Converts::SWING_MODE_STR = {
    { SWING_OFF, "Off" },
    { SWING_AUTO, "Auto" },
    { SWING_TOP, "Top" },
    { SWING_HIGHEST, "Highest" },
    { SWING_HIGH, "High" },
    { SWING_MIDDLE, "Middle" },
    { SWING_LOW, "Low" },
    { SWING_LOWEST, "Lowest" },
};

std::map<HaierAC160FanSpeed, std::string>
Converts::FAN_SPEED_STR = {
    { SPEED_AUTO, "Auto" },
    { SPEED_LOW, "Low" },
    { SPEED_MEDIUM, "Medium" },
    { SPEED_HIGH, "High" },
};

std::optional<std::string> Converts::get_operate_mode_str(
        HaierAC160OperateMode op_mode) {
    return get_enum_str(op_mode, OPERATE_MODE_STR);
}

std::optional<std::string> Converts::get_swing_mode_str(
        HaierAC160SwingMode swing_mode) {
    return get_enum_str(swing_mode, SWING_MODE_STR);
}

std::optional<std::string> Converts::get_fan_speed_str(
        HaierAC160FanSpeed fan_speed) {
    return get_enum_str(fan_speed, FAN_SPEED_STR);
}

std::optional<HaierAC160OperateMode>
Converts::get_operate_mode_by_str(std::string operate_mode_str) {
    return get_enum_by_str(operate_mode_str, OPERATE_MODE_STR);
}

std::optional<HaierAC160SwingMode>
Converts::get_swing_mode_by_str(std::string swing_mode_str) {
    return get_enum_by_str(swing_mode_str, SWING_MODE_STR);
}

std::optional<HaierAC160FanSpeed>
Converts::get_fan_speed_by_str(std::string fan_speed_str) {
    return get_enum_by_str(fan_speed_str, FAN_SPEED_STR);
}

} // namespace esphome
} // namespace haier_ac160
