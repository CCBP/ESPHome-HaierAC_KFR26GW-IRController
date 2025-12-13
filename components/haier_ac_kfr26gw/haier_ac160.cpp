#include "haier_ac160.h"

namespace esphome {
namespace haier_ac160 {

static const char *const TAG = "haier_ac160";

// Random 32bit value; If this changes existing restore preferences are invalidated
static const uint32_t RESTORE_STATE_VERSION = 0xA02E6FA4UL;

static const std::string TIMER_OFF_STR = "--";

void HaierAC160::init(uint16_t pin,
        const bool need_restore, const bool inverted) {
    ac_ = new IRHaierAC160(pin, inverted);
    ac_->begin();

    this->need_restore_ = need_restore;
    if (this->need_restore_) {
        this->restore_state_();
    } else {
        ESP_LOGI(TAG, "The HaierAC160 protocol recovery has been disabled.");
        ac_->stateReset();
    }

    ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
    ESP_LOGD(TAG, "  %s\n", ac_->toString().c_str());
}

bool HaierAC160::restore_state_() {
    if (!this->need_restore_) return false;

    this->rtc_ = global_preferences->make_preference<HaierAc160Protocol>(
        this->get_preference_hash() ^ RESTORE_STATE_VERSION
    );
    HaierAc160Protocol protocol{};
    if (this->rtc_.load(&protocol)) {
        ac_->setRaw(protocol.raw);
        ESP_LOGI(TAG, "HaierAC160 protocol have been restored.");
        this->temperature_nu_->make_call().set_value(ac_->getTemp());
        this->power_sw_->control(ac_->getPower());
        this->sleep_sw_->control(ac_->getSleep());
        this->lock_sw_->control(ac_->getLock());
        this->display_sw_->control(ac_->getLightToggle());
        this->aux_heating_sw_->control(ac_->getAuxHeating());
        this->self_clean_sw_->control(ac_->getClean());
        this->turbo_sw_->control(ac_->getTurbo());
        this->quiet_sw_->control(ac_->getQuiet());
        this->health_sw_->control(ac_->getHealth());
        this->operate_mode_se_->make_call().set_option(
            Converts::get_operate_mode_str(
                static_cast<HaierAC160OperateMode>(ac_->getMode()))
        );
        this->fan_speed_se_->make_call().set_option(
            Converts::get_fan_speed_str(
                static_cast<HaierAC160FanSpeed>(ac_->getFan()))
        );
        this->swing_mode_se_->make_call().set_option(
            Converts::get_swing_mode_str(
                static_cast<HaierAC160SwingMode>(ac_->getSwingV()))
        );
        ac_->setTimerMode(kHaierAcYrw02NoTimers);
        this->on_timer_hour_se_->make_call().set_index(0);
        this->on_timer_minute_se_->make_call().set_index(0);
        this->off_timer_hour_se_->make_call().set_index(0);
        this->off_timer_minute_se_->make_call().set_index(0);
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to restore HaierAC160 protocol.");
        return false;
    }
}

void HaierAC160::perform(bool ignore_power) {
    if (ignore_power || ac_->getPower()) ac_->send();
    if (this->need_restore_) {
        if (this->rtc_.save(ac_->getRaw()))
            ESP_LOGD(TAG, "Current state has been saved.");
        else
            ESP_LOGE(TAG, "Failed to save current state.");
    }

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
        this->perform(true);
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

void HaierAC160::self_clean_switch_handler(bool state) {
    ESP_LOGD(TAG, "Self Clean switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getClean()) {
        ac_->setClean(state);
        this->perform();
    }
}

void HaierAC160::set_self_clean_switch(HaierAC160Switch *self_clean_sw) {
    this->self_clean_sw_ = self_clean_sw;
    this->self_clean_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->self_clean_switch_handler(state);
        }
    );
}

void HaierAC160::turbo_switch_handler(bool state) {
    ESP_LOGD(TAG, "Turbo switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getTurbo()) {
        ac_->setTurbo(state);
        this->perform();
    }
}

void HaierAC160::set_turbo_switch(HaierAC160Switch *turbo_sw) {
    this->turbo_sw_ = turbo_sw;
    this->turbo_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->turbo_switch_handler(state);
        }
    );
}

void HaierAC160::quiet_switch_handler(bool state) {
    ESP_LOGD(TAG, "Quiet switch changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getQuiet()) {
        ac_->setQuiet(state);
        this->perform();
    }
}

void HaierAC160::set_quiet_switch(HaierAC160Switch *quiet_sw) {
    this->quiet_sw_ = quiet_sw;
    this->quiet_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->quiet_switch_handler(state);
        }
    );
}

void HaierAC160::health_switch_handler(bool state) {
    ESP_LOGD(TAG, "Health switch changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getHealth()) {
        ac_->setHealth(state);
        this->perform();
    }
}

void HaierAC160::set_health_switch(HaierAC160Switch *health_sw) {
    this->health_sw_ = health_sw;
    this->health_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->health_switch_handler(state);
        }
    );
}

void HaierAC160::operate_mode_select_handler(
        HaierAC160OperateMode op_mode) {
    if (op_mode != ac_->getMode()) {
        ac_->setMode(op_mode);
        this->perform();
    }
}

void HaierAC160::set_operate_mode_select(HaierAC160Select *operate_mode_se) {
    this->operate_mode_se_ = operate_mode_se;
    this->operate_mode_se_->set_callback_handler(
        [this](const std::string &op_mode_str) -> void {
            ESP_LOGD(TAG, "Operate Mode was selected as %s",
                op_mode_str.c_str());

            auto op_mode = Converts::get_operate_mode_by_str(op_mode_str);
            if (op_mode.has_value())
                this->operate_mode_select_handler(*op_mode);
            else
                ESP_LOGE(TAG, "Operate Mode '%s' is invalid", op_mode_str);
        }
    );
}

void HaierAC160::swing_mode_select_handler(
        HaierAC160SwingMode swing_mode) {
    if (swing_mode != ac_->getSwingV()) {
        ac_->setSwingV(swing_mode);
        this->perform();
    }
}

void HaierAC160::set_swing_mode_select(HaierAC160Select *swing_mode_se) {
    this->swing_mode_se_ = swing_mode_se;
    this->swing_mode_se_->set_callback_handler(
        [this](const std::string &swing_mode_str) -> void {
            ESP_LOGD(TAG, "Swing Mode was selected as %s",
                swing_mode_str.c_str());

            auto swing_mode = Converts::get_swing_mode_by_str(swing_mode_str);
            if (swing_mode.has_value())
                this->swing_mode_select_handler(*swing_mode);
            else
                ESP_LOGE(TAG, "Swing Mode '%s' is invalid", swing_mode_str);
        }
    );
}

void HaierAC160::fan_speed_select_handler(
        HaierAC160FanSpeed fan_speed) {
    if (fan_speed != ac_->getFan()) {
        ac_->setFan(fan_speed);
        this->perform();
    }
}

void HaierAC160::set_fan_speed_select(HaierAC160Select *fan_speed_se) {
    this->fan_speed_se_ = fan_speed_se;
    this->fan_speed_se_->set_callback_handler(
        [this](const std::string &fan_speed_str) -> void {
            ESP_LOGD(TAG, "Fan Speed was selected as %s",
                fan_speed_str.c_str());

            auto fan_speed = Converts::get_fan_speed_by_str(fan_speed_str);
            if (fan_speed.has_value())
                this->fan_speed_select_handler(*fan_speed);
            else
                ESP_LOGE(TAG, "Fan Speed '%s' is invalid", fan_speed_str);
        }
    );
}

void HaierAC160::on_timer_select_handler() {
    uint16_t total_mins = this->on_timer_hour_num * 60 +
        this->on_timer_minute_num;
    ESP_LOGD(TAG, "On Timer wae select as %02d:%02d, "
            "The AC will turn off in %d minutes.",
            this->on_timer_hour_num, this->on_timer_minute_num,
            total_mins);

    ac_->setOnTimer(total_mins);
    this->perform(true);

    this->set_timeout("on_timer", total_mins * 60 * 1000, [this]() {
        ESP_LOGI(TAG, "The air conditioner has been turned on.");
        this->on_timer_hour_se_->make_call().set_index(0);
        this->on_timer_minute_se_->make_call().set_index(0);
        ac_->setPower(true);
        this->power_sw_->control(ac_->getPower());
    });
}

void HaierAC160::set_on_timer_hour_select(HaierAC160Select *on_timer_hour_se) {
    this->on_timer_hour_se_ = on_timer_hour_se;
    this->on_timer_hour_se_->set_callback_handler(
        [this](const std::string &hour_str) -> void {
            ESP_LOGD(TAG,
                "On Timer Hour was selected as %s", hour_str.c_str());

            if (hour_str == TIMER_OFF_STR) this->on_timer_hour_num = 0;
            else this->on_timer_hour_num = std::stoi(hour_str);
            this->on_timer_select_handler();
        }
    );
}

void HaierAC160::set_on_timer_minute_select(HaierAC160Select *on_timer_minute_se) {
    this->on_timer_minute_se_ = on_timer_minute_se;
    this->on_timer_minute_se_->set_callback_handler(
        [this](const std::string &min_str) -> void {
            ESP_LOGD(TAG,
                "On Timer Minute was selected as %s", min_str.c_str());

            if (min_str == TIMER_OFF_STR) this->on_timer_minute_num = 0;
            else this->on_timer_minute_num = std::stoi(min_str);
            this->on_timer_select_handler();
        }
    );
}

void HaierAC160::off_timer_select_handler() {
    uint16_t total_mins = this->off_timer_hour_num * 60 +
        this->off_timer_minute_num;
    ESP_LOGD(TAG, "Off Timer wae select as %02d:%02d, "
            "The AC will turn off in %d minutes.",
            this->off_timer_hour_num, this->off_timer_minute_num,
            total_mins);

    ac_->setOffTimer(total_mins);
    this->perform();

    this->set_timeout("off_timer", total_mins * 60 * 1000, [this]() {
        ESP_LOGI(TAG, "The air conditioner has been turned off.");
        this->off_timer_hour_se_->make_call().set_index(0);
        this->off_timer_minute_se_->make_call().set_index(0);
        ac_->setPower(false);
        this->power_sw_->control(ac_->getPower());
    });
}

void HaierAC160::set_off_timer_hour_select(HaierAC160Select *off_timer_hour_se) {
    this->off_timer_hour_se_ = off_timer_hour_se;
    this->off_timer_hour_se_->set_callback_handler(
        [this](const std::string &hour_str) -> void {
            ESP_LOGD(TAG, "Off Timer Hour was selected as %s", hour_str.c_str());

            if (hour_str == TIMER_OFF_STR) this->off_timer_hour_num = 0;
            else this->off_timer_hour_num = std::stoi(hour_str);
            this->off_timer_select_handler();
        }
    );
}

void HaierAC160::set_off_timer_minute_select(HaierAC160Select *off_timer_minute_se) {
    this->off_timer_minute_se_ = off_timer_minute_se;
    this->off_timer_minute_se_->set_callback_handler(
        [this](const std::string &min_str) -> void {
            ESP_LOGD(TAG, "Off Timer Minute was selected as %s", min_str.c_str());

            if (min_str == TIMER_OFF_STR) this->off_timer_minute_num = 0;
            else this->off_timer_minute_num = std::stoi(min_str);
            this->off_timer_select_handler();
        }
    );
}

}  // namespace haier_ac160
}  // namespace esphome
