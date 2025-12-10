#pragma once

#include <map>

#include "ir_Haier.h"

namespace esphome {
namespace haier_ac160 {

enum HaierAC160SwingMode : uint8_t {
    SWING_OFF = kHaierAc160SwingVOff,
    SWING_AUTO = kHaierAc160SwingVAuto,
    SWING_TOP = kHaierAc160SwingVTop,
    SWING_HIGHEST = kHaierAc160SwingVHighest,
    SWING_HIGH = kHaierAc160SwingVHigh,
    SWING_MIDDLE = kHaierAc160SwingVMiddle,
    SWING_LOW = kHaierAc160SwingVLow,
    SWING_LOWEST = kHaierAc160SwingVLowest,
};

enum HaierAC160FanSpeed : uint8_t {
    SPEED_AUTO = kHaierAcYrw02FanAuto,
    SPEED_LOW = kHaierAcYrw02FanLow,
    SPEED_MEDIUM = kHaierAcYrw02FanMed,
    SPEED_HIGH = kHaierAcYrw02FanAuto,
};

class Converts {
    public:
        template <typename EnumT>
        static const char *const get_enum_str(EnumT key,
                const std::map<EnumT, const char *> &table) {
            auto it = table.find(key);
            if (it != table.end()) {
                return it->second;
            }
            return "UNKNOW";
        }

        static const char *const get_swing_mode_str(HaierAC160SwingMode swing_mode);
        static const char *const get_fan_speed_str(HaierAC160FanSpeed fan_speed);

    private:
        static std::map<HaierAC160SwingMode, const char *> SWING_MODE_STR;
        static std::map<HaierAC160FanSpeed, const char *> FAN_SPEED_STR;
};

} // namespace haier_ac160
} // namespace esphome
