#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "haier_ac_adapter.h"
#include "haier_ac_utils.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"

namespace esphome {
namespace haier_ac160 {

class HaierAC160 : public Component {
    public:
        HaierAC160() {}

        void init(uint16_t pin, const bool recovery = true,
            const bool inverted = false);

        void perform();

        void set_temperature_number(HaierAC160Number *temperature_nu);

        void set_power_switch(HaierAC160Switch *power_sw);
        void set_sleep_switch(HaierAC160Switch *sleep_switch);
        void set_lock_switch(HaierAC160Switch *lock_switch);
        void set_display_switch(HaierAC160Switch *display_sw);
        void set_aux_heating_switch(HaierAC160Switch *aux_heating);

        void set_operate_mode_select(HaierAC160Select *operate_mode_se_);
        void set_swing_mode_select(HaierAC160Select *swing_mode_se);
        void set_fan_speed_select(HaierAC160Select *fan_speed_se);
        void set_timer_hour_step(uint8_t step)
            { this->timer_hour_step = step; }
        void set_timer_hour_select(HaierAC160Select *timer_hour_se);
        void set_timer_minute_step(uint8_t step)
            { this->timer_minute_step = step; }
        void set_timer_minute_select(HaierAC160Select *timer_minute_se);

    protected:
        IRHaierAC160 *ac_{nullptr};

        HaierAC160Number *temperature_nu_{nullptr};

        HaierAC160Switch *power_sw_{nullptr};
        HaierAC160Switch *sleep_sw_{nullptr};
        HaierAC160Switch *lock_sw_{nullptr};
        HaierAC160Switch *display_sw_{nullptr};
        HaierAC160Switch *aux_heating_sw_{nullptr};

        HaierAC160Select *operate_mode_se_{nullptr};
        HaierAC160Select *fan_speed_se_{nullptr};
        HaierAC160Select *swing_mode_se_{nullptr};
        uint8_t timer_hour_step{1};
        uint8_t timer_hour_num{0};
        HaierAC160Select *timer_hour_se_{nullptr};
        uint8_t timer_minute_step{1};
        uint8_t timer_minute_num{0};
        HaierAC160Select *timer_minute_se_{nullptr};

        void temperature_number_handler(uint8_t temp);

        void power_switch_handler(bool state);
        void sleep_switch_handler(bool state);
        void lock_switch_handler(bool state);
        void display_switch_handler(bool state);
        void aux_heating_switch_handler(bool state);

        void operate_mode_select_handler(HaierAC160OperateMode op_mode);
        void swing_mode_select_handler(HaierAC160SwingMode swing_mode);
        void fan_speed_select_handler(HaierAC160FanSpeed fan_speed);
        void timer_select_handler();
        void timer_hour_select_handler(uint8_t hour);
        void timer_minute_select_handler(uint8_t minute);
};

}  // namespace haier_ac160
}  // namespace esphome
