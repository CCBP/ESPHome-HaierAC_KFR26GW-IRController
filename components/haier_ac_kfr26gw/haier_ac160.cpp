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
        if (ac_->getPower())
            ac_->send();
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
        if (ac_->getPower())
            ac_->send();
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

void HaierAC160::swing_mode_select_handler(
        HaierAC160SwingMode swing_mode) {
    ESP_LOGD(TAG, "Swing Mode was selected as %s",
        get_swing_mode_str(swing_mode));
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
        get_fan_speed_str(fan_speed));
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

void HaierAC160::timer_hour_select_handler(uint8_t hour) {
    ESP_LOGD(TAG, "Timer set to hour %d", hour);
}

void HaierAC160::set_timer_hour_select(
        HaierAC160Select<uint8_t> *timer_hour_se) {
    this->timer_hour_se_ = timer_hour_se;
    this->timer_hour_se_->set_callback_handler(
        [this](uint8_t hour) -> void {
            this->timer_hour_select_handler(hour);
        }
    );
}

void HaierAC160::timer_minute_select_handler(uint8_t minute) {
    ESP_LOGD(TAG, "Timer set to minute %d", minute);
}

void HaierAC160::set_timer_minute_select(
        HaierAC160Select<uint8_t> *timer_minute_se) {
    this->timer_minute_se_ = timer_minute_se;
    this->timer_minute_se_->set_callback_handler(
        [this](uint8_t minute) -> void {
            this->timer_minute_select_handler(minute);
        }
    );
}

}  // namespace haier_ac160
}  // namespace esphome
