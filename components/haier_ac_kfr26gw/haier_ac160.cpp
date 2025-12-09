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
