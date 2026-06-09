#include "haier_ac_utils.h"

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

} // namespace esphome
} // namespace haier_ac160
