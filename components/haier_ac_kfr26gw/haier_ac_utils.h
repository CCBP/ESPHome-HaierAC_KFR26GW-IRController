#pragma once

#include <map>

#include "esphome/core/log.h"
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
    SPEED_HIGH = kHaierAcYrw02FanHigh,
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
            for (const auto &entry : table) {
                if (str_equals_case_insensitive(str, entry.second))
                    return entry.first;
            }
            return std::nullopt;
        }

        static std::optional<std::string>
            get_operate_mode_str(HaierAC160OperateMode op_mode) {
            return get_enum_str(op_mode, OPERATE_MODE_STR);
        }
        static std::optional<std::string>
            get_swing_mode_str(HaierAC160SwingMode swing_mode) {
            return get_enum_str(swing_mode, SWING_MODE_STR);
        }
        static std::optional<std::string>
            get_fan_speed_str(HaierAC160FanSpeed fan_speed) {
            return get_enum_str(fan_speed, FAN_SPEED_STR);
        }

        static std::optional<HaierAC160OperateMode>
            get_operate_mode_by_str(const std::string &str) {
            return get_enum_by_str(str, OPERATE_MODE_STR);
        }
        static std::optional<HaierAC160SwingMode>
            get_swing_mode_by_str(const std::string &str) {
            return get_enum_by_str(str, SWING_MODE_STR);
        }
        static std::optional<HaierAC160FanSpeed>
            get_fan_speed_by_str(const std::string &str) {
            return get_enum_by_str(str, FAN_SPEED_STR);
        }
};

} // namespace haier_ac160
} // namespace esphome
