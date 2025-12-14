#pragma once

#include <map>

#include "esphome/core/helpers.h"

#include "ir_Haier.h"

namespace esphome {
namespace haier_ac160 {

struct HaierAC160ProtocolRestore {
    uint8_t state[kHaierAC160StateLength];
};

enum HaierAC160OperateMode : uint8_t {
    MODE_AUTO = kHaierAcYrw02Auto,
    MODE_COOL = kHaierAcYrw02Cool,
    MODE_HEAT = kHaierAcYrw02Heat,
    MODE_DRY = kHaierAcYrw02Dry,
    MODE_FAN = kHaierAcYrw02Fan,
};

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
        static std::map<HaierAC160OperateMode, std::string> OPERATE_MODE_STR;
        static std::map<HaierAC160SwingMode, std::string> SWING_MODE_STR;
        static std::map<HaierAC160FanSpeed, std::string> FAN_SPEED_STR;

        template <typename EnumT>
        static std::optional<std::string>
        get_enum_str(EnumT key, 
                const std::map<EnumT, std::string> &table) {
            auto it = table.find(key);
            if (it != table.end()) {
                return it->second;
            }
            return std::nullopt;
        }

        template <typename EnumT>
        static std::optional<EnumT>
        get_enum_by_str(const std::string &str,
                const std::map<EnumT, std::string> &table) {
            for (const auto table : table) {
                if (str_equals_case_insensitive(str, table.second))
                    return table.first;
            }
            return std::nullopt;
        }

        static std::optional<std::string>
            get_operate_mode_str(HaierAC160OperateMode op_mode);
        static std::optional<std::string>
            get_swing_mode_str(HaierAC160SwingMode swing_mode);
        static std::optional<std::string>
            get_fan_speed_str(HaierAC160FanSpeed fan_speed);

        static std::optional<HaierAC160OperateMode>
            get_operate_mode_by_str(std::string op_mode_str);
        static std::optional<HaierAC160SwingMode>
            get_swing_mode_by_str(std::string swing_mode_str);
        static std::optional<HaierAC160FanSpeed>
            get_fan_speed_by_str(std::string fan_speed_str);
};

} // namespace haier_ac160
} // namespace esphome
