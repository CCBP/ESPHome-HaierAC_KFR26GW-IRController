#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"
#include "esphome/core/log.h"

#include "haier_ac_adapter.h"
#include "haier_ac_utils.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

namespace esphome {
namespace haier_ac160 {

class HaierAC160 : public Component, public EntityBase {
    public:
        HaierAC160() {}

        void init(uint16_t pin, const bool recovery = true,
            const bool inverted = false);

        bool restore_state_();
        void perform(bool ignore_power = false);

        void set_temperature_number(HaierAC160Number *temperature_nu);

        void set_power_switch(HaierAC160Switch *power_sw);
        void set_sleep_switch(HaierAC160Switch *sleep_switch);
        void set_lock_switch(HaierAC160Switch *lock_switch);
        void set_display_switch(HaierAC160Switch *display_sw);
        void set_aux_heating_switch(HaierAC160Switch *aux_heating_sw);
        void set_self_clean_switch(HaierAC160Switch *self_clean_sw);
        void set_turbo_switch(HaierAC160Switch *turbo_sw);
        void set_quiet_switch(HaierAC160Switch *quiet_sw);
        void set_health_switch(HaierAC160Switch *health_sw);

        void set_operate_mode_select(HaierAC160Select *operate_mode_se_);
        void set_swing_mode_select(HaierAC160Select *swing_mode_se);
        void set_fan_speed_select(HaierAC160Select *fan_speed_se);
        void set_on_timer_hour_select(HaierAC160Select *on_timer_hour_se);
        void set_on_timer_minute_select(HaierAC160Select *on_timer_minute_se);
        void set_off_timer_hour_select(HaierAC160Select *off_timer_hour_se);
        void set_off_timer_minute_select(HaierAC160Select *off_timer_minute_se);

    protected:
        IRHaierAC160 *ac_{nullptr};
        ESPPreferenceObject rtc_;
        bool need_restore_{false};

        HaierAC160Number *temperature_nu_{nullptr};

        HaierAC160Switch *power_sw_{nullptr};
        HaierAC160Switch *sleep_sw_{nullptr};
        HaierAC160Switch *lock_sw_{nullptr};
        HaierAC160Switch *display_sw_{nullptr};
        HaierAC160Switch *aux_heating_sw_{nullptr};
        HaierAC160Switch *self_clean_sw_{nullptr};
        HaierAC160Switch *turbo_sw_{nullptr};
        HaierAC160Switch *quiet_sw_{nullptr};
        HaierAC160Switch *health_sw_{nullptr};

        HaierAC160Select *operate_mode_se_{nullptr};
        HaierAC160Select *fan_speed_se_{nullptr};
        HaierAC160Select *swing_mode_se_{nullptr};
        uint8_t on_timer_hour_num{0};
        HaierAC160Select *on_timer_hour_se_{nullptr};
        uint8_t on_timer_minute_num{0};
        HaierAC160Select *on_timer_minute_se_{nullptr};
        uint8_t off_timer_hour_num{0};
        HaierAC160Select *off_timer_hour_se_{nullptr};
        uint8_t off_timer_minute_num{0};
        HaierAC160Select *off_timer_minute_se_{nullptr};

        void temperature_number_handler(uint8_t temp);

        void power_switch_handler(bool state);
        void sleep_switch_handler(bool state);
        void lock_switch_handler(bool state);
        void display_switch_handler(bool state);
        void aux_heating_switch_handler(bool state);
        void self_clean_switch_handler(bool state);
        void turbo_switch_handler(bool state);
        void quiet_switch_handler(bool state);
        void health_switch_handler(bool state);

        void operate_mode_select_handler(HaierAC160OperateMode op_mode);
        void swing_mode_select_handler(HaierAC160SwingMode swing_mode);
        void fan_speed_select_handler(HaierAC160FanSpeed fan_speed);
        void disable_on_timer();
        void on_timer_select_handler();
        void on_timer_hour_select_handler(uint8_t hour);
        void on_timer_minute_select_handler(uint8_t minute);
        void disable_off_timer();
        void off_timer_select_handler();
        void off_timer_hour_select_handler(uint8_t hour);
        void off_timer_minute_select_handler(uint8_t minute);
};

}  // namespace haier_ac160
}  // namespace esphome
