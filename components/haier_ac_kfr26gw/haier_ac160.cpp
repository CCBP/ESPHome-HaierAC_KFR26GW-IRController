#include "haier_ac160.h"

namespace esphome {
namespace haier_ac160 {

static const char *const TAG = "haier_ac160";

void HaierAC160::init(uint16_t pin,
        const bool recovery, const bool inverted) {
    if (recovery) {
        ESP_LOGI(TAG, "Configuration items have been restored");
    } else {
        ESP_LOGI(TAG,
            "The configuration was not restored because it was %s but %s.",
            recovery ? "enabled" : "disabled");
    }

    ac_ = new IRHaierAC160(pin, inverted);
    ac_->begin();
    ac_->stateReset();

    ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
    ESP_LOGD(TAG, "  %s\n", ac_->toString().c_str());
}

void HaierAC160::perform() {
    if (ac_->getPower())
        ac_->send();

    ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
    ESP_LOGD(TAG, "  %s", ac_->toString().c_str());
}

void HaierAC160::temperature_number_handler(uint8_t temp) {
    ESP_LOGD(TAG, "Temperature was set to %d", temp);

    if (temp != ac_->getTemp()) {
        ac_->setTemp(temp);
        this->perform();
    }
}

void HaierAC160::set_temperature_number(HaierAC160Number *temperature_nu) {
    this->temperature_nu_ = temperature_nu;
    this->temperature_nu_->set_callback_handler(
        [this](float temp) -> void {
            this->temperature_number_handler(static_cast<uint8_t>(temp));
        }
    );
}

void HaierAC160::power_switch_handler(bool state) {
    ESP_LOGD(TAG, "Power switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getPower()) {
        ac_->setPower(state);
        this->perform();
    }
}

void HaierAC160::set_power_switch(HaierAC160Switch *power_sw) {
    this->power_sw_ = power_sw;
    this->power_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->power_switch_handler(state);
        }
    );
}

void HaierAC160::sleep_switch_handler(bool state) {
    ESP_LOGD(TAG, "Sleep switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getSleep()) {
        ac_->setSleep(state);
        this->perform();
    }
}

void HaierAC160::set_sleep_switch(HaierAC160Switch *sleep_sw) {
    this->sleep_sw_ = sleep_sw;
    this->sleep_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->sleep_switch_handler(state);
        }
    );
}

void HaierAC160::lock_switch_handler(bool state) {
    ESP_LOGD(TAG, "Lock switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getLock()) {
        ac_->setLock(state);
        this->perform();
    }
}

void HaierAC160::set_lock_switch(HaierAC160Switch *lock_sw) {
    this->lock_sw_ = lock_sw;
    this->lock_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->lock_switch_handler(state);
        }
    );
}

void HaierAC160::display_switch_handler(bool state) {
    ESP_LOGD(TAG, "Display switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getLightToggle()) {
        ac_->setLightToggle(state);
        this->perform();
    }
}

void HaierAC160::set_display_switch(HaierAC160Switch *display_sw) {
    this->display_sw_ = display_sw;
    this->display_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->display_switch_handler(state);
        }
    );
}

void HaierAC160::aux_heating_switch_handler(bool state) {
    ESP_LOGD(TAG, "Auxiliary Heating switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getAuxHeating()) {
        ac_->setAuxHeating(state);
        this->perform();
    }
}

void HaierAC160::set_aux_heating_switch(
        HaierAC160Switch *aux_heating_sw) {
    this->aux_heating_sw_ = aux_heating_sw;
    this->aux_heating_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->aux_heating_switch_handler(state);
        }
    );
}

void HaierAC160::operate_mode_select_handler(
        HaierAC160OperateMode op_mode) {
    ESP_LOGD(TAG, "Operate Mode was selected as %s",
        Converts::get_operate_mode_str(op_mode));

    if (op_mode != ac_->getMode()) {
        ac_->setMode(op_mode);
        this->perform();
    }
}

void HaierAC160::set_operate_mode_select(
        HaierAC160Select<HaierAC160OperateMode> *operate_mode_se) {
    this->operate_mode_se_ = operate_mode_se;
    this->operate_mode_se_->set_callback_handler(
        [this](HaierAC160OperateMode op_mode) -> void {
            this->operate_mode_select_handler(op_mode);
        }
    );
}

void HaierAC160::swing_mode_select_handler(
        HaierAC160SwingMode swing_mode) {
    ESP_LOGD(TAG, "Swing Mode was selected as %s",
        Converts::get_swing_mode_str(swing_mode));

    if (swing_mode != ac_->getSwingV()) {
        ac_->setSwingV(swing_mode);
        this->perform();
    }
}

void HaierAC160::set_swing_mode_select(
        HaierAC160Select<HaierAC160SwingMode> *swing_mode_se) {
    this->swing_mode_se_ = swing_mode_se;
    this->swing_mode_se_->set_callback_handler(
        [this](HaierAC160SwingMode swing_mode) -> void {
            this->swing_mode_select_handler(swing_mode);
        }
    );
}

void HaierAC160::fan_speed_select_handler(
        HaierAC160FanSpeed fan_speed) {
    ESP_LOGD(TAG, "Fan Speed was selected as %s",
        Converts::get_fan_speed_str(fan_speed));

    if (fan_speed != ac_->getFan()) {
        ac_->setFan(fan_speed);
        this->perform();
    }
}

void HaierAC160::set_fan_speed_select(
        HaierAC160Select<HaierAC160FanSpeed> *fan_speed_se) {
    this->fan_speed_se_ = fan_speed_se;
    this->fan_speed_se_->set_callback_handler(
        [this](HaierAC160FanSpeed fan_speed) -> void {
            this->fan_speed_select_handler(fan_speed);
        }
    );
}

void HaierAC160::timer_select_handler() {
    uint8_t hour = this->timer_hour_se_->active_index().value_or(0);
    if (hour != 0) // subtract "--" from the index
        hour = this->timer_hour_step * (hour - 1);
    uint8_t minute = this->timer_minute_se_->active_index().value_or(0);
    if (minute != 0) // subtract "--" from the index
        minute = this->timer_minute_step * (minute - 1);
    uint16_t total_mins = hour * 60 + minute;
    ESP_LOGD(TAG, "Timer wae select as %02d:%02d, "
            "The AC will turn off in %d minutes.",
            hour, minute, total_mins);

    ac_->setOffTimer(total_mins);
    this->perform();

    if (total_mins == 0) {
        this->timer_hour_se_->make_call().set_index(0);
        this->timer_minute_se_->make_call().set_index(0);
    }
}

void HaierAC160::set_timer_hour_select(
        HaierAC160Select<uint8_t> *timer_hour_se) {
    this->timer_hour_se_ = timer_hour_se;
    this->timer_hour_se_->set_callback_handler(
        [this](uint8_t hour) -> void {
            const char *hour_str = this->timer_hour_se_->option_at(hour);
            ESP_LOGD(TAG, "Timer Hour was selected as %s with step %d",
                hour_str, this->timer_hour_step);

            this->timer_select_handler();
        }
    );
}

void HaierAC160::set_timer_minute_select(
        HaierAC160Select<uint8_t> *timer_minute_se) {
    this->timer_minute_se_ = timer_minute_se;
    this->timer_minute_se_->set_callback_handler(
        [this](uint8_t minute) -> void {
            const char *min_str = this->timer_minute_se_->option_at(minute);
            ESP_LOGD(TAG, "Timer Minute was selected as %s with step %d",

                min_str, this->timer_minute_step);
            this->timer_select_handler();
        }
    );
}

}  // namespace haier_ac160
}  // namespace esphome
