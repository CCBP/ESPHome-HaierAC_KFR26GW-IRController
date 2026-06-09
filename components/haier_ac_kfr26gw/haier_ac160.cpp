#include "haier_ac160.h"

#include <memory>

namespace esphome {
namespace haier_ac160 {

static const char *const TAG = "haier_ac160";

// Random 32bit value; If this changes existing restore preferences are invalidated
static const uint32_t RESTORE_STATE_VERSION = 0xB01E7FA4UL;

static const std::string TIMER_OFF_STR = "--";

void HaierAC160::init(uint16_t pin,
        const bool restore_state, const bool inverted) {
    ac_ = std::make_unique<IRHaierAC160>(pin, inverted);
    ac_->begin();

    this->need_restore_ = restore_state;
    if (!this->need_restore_ || !this->restore_state_()) {
        ESP_LOGW(TAG,
            "Since restore is %s, the default settings were used.",
            this->need_restore_ ? "failed" : "disabled");
        ac_->stateReset();
    }

    this->sync_entities_();

    ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
    ESP_LOGD(TAG, "  %s\n", ac_->toString().c_str());
}

void HaierAC160::sync_entities_() {
    this->temperature_nu_->make_call()
        .set_value(ac_->getTemp()).perform();
    this->power_sw_->control(ac_->getPower());
    this->sleep_sw_->control(ac_->getSleep());
    this->lock_sw_->control(ac_->getLock());
    this->display_sw_->control(ac_->getLightToggle());
    this->aux_heating_sw_->control(ac_->getAuxHeating());
    this->self_clean_sw_->control(ac_->getClean());
    this->turbo_sw_->control(ac_->getTurbo());
    this->quiet_sw_->control(ac_->getQuiet());
    this->health_sw_->control(ac_->getHealth());
    this->operate_mode_se_->make_call().with_option(
        Converts::get_operate_mode_str(
            static_cast<HaierAC160OperateMode>(ac_->getMode())
        ).value_or(Converts::OPERATE_MODE_STR[MODE_AUTO])
    ).perform();
    this->fan_speed_se_->make_call().with_option(
        Converts::get_fan_speed_str(
            static_cast<HaierAC160FanSpeed>(ac_->getFan())
        ).value_or(Converts::FAN_SPEED_STR[SPEED_AUTO])
    ).perform();
    this->swing_mode_se_->make_call().with_option(
        Converts::get_swing_mode_str(
            static_cast<HaierAC160SwingMode>(ac_->getSwingV())
        ).value_or(Converts::SWING_MODE_STR[SWING_AUTO])
    ).perform();
    ac_->setTimerMode(kHaierAcYrw02NoTimers);
    this->on_timer_hour_se_->make_call()
        .with_index(0).perform();
    this->on_timer_minute_se_->make_call()
        .with_index(0).perform();
    this->off_timer_hour_se_->make_call()
        .with_index(0).perform();
    this->off_timer_minute_se_->make_call()
        .with_index(0).perform();
}

bool HaierAC160::restore_state_() {
    if (!this->need_restore_) return false;

    this->rtc_ = global_preferences->make_preference<HaierAC160ProtocolRestore>(
        this->get_preference_hash() ^ RESTORE_STATE_VERSION
    );
    HaierAC160ProtocolRestore protocol{};
    if (this->rtc_.load(&protocol)) {
        ac_->setRaw(protocol.state);
        ESP_LOGI(TAG, "HaierAC160 protocol have been restored.");
        ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
        ESP_LOGD(TAG, "  %s\n", ac_->toString().c_str());
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to restore HaierAC160 protocol.");
        return false;
    }
}

void HaierAC160::perform(bool ignore_power) {
    if (ignore_power || ac_->getPower()) ac_->send();
    if (this->need_restore_) {
        if (this->rtc_.save(reinterpret_cast<HaierAC160ProtocolRestore *>(ac_->getRaw())))
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

void HaierAC160::set_switch_(HaierAC160Switch *sw, const char *name,
        BoolGetter getter, BoolSetter setter, bool ignore_power) {
    sw->set_callback_handler(
        [this, name, getter, setter, ignore_power](bool state) -> void {
            ESP_LOGD(TAG, "%s switch state changed to %s",
                name, state ? "ON" : "OFF");

            if (state != getter()) {
                setter(state);
                this->perform(ignore_power);
            }
        }
    );
}

void HaierAC160::power_switch_handler(bool state) {
    ESP_LOGD(TAG, "Power switch state changed to %s",
        state ? "ON" : "OFF");

    if (state != ac_->getPower()) {
        /**
         * If the power is operated manually,
         * all timers will be turned off.
         */
        this->disable_on_timer();
        this->disable_off_timer();

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

void HaierAC160::set_sleep_switch(HaierAC160Switch *sleep_sw) {
    this->sleep_sw_ = sleep_sw;
    this->set_switch_(sleep_sw, "Sleep",
        [this]() { return ac_->getSleep(); },
        [this](bool v) { ac_->setSleep(v); });
}

void HaierAC160::set_lock_switch(HaierAC160Switch *lock_sw) {
    this->lock_sw_ = lock_sw;
    this->set_switch_(lock_sw, "Lock",
        [this]() { return ac_->getLock(); },
        [this](bool v) { ac_->setLock(v); });
}

void HaierAC160::set_display_switch(HaierAC160Switch *display_sw) {
    this->display_sw_ = display_sw;
    // getLightToggle()/setLightToggle() controls the AC display on/off
    this->set_switch_(display_sw, "Display",
        [this]() { return ac_->getLightToggle(); },
        [this](bool v) { ac_->setLightToggle(v); });
}

void HaierAC160::set_aux_heating_switch(HaierAC160Switch *aux_heating_sw) {
    this->aux_heating_sw_ = aux_heating_sw;
    this->set_switch_(aux_heating_sw, "Auxiliary Heating",
        [this]() { return ac_->getAuxHeating(); },
        [this](bool v) { ac_->setAuxHeating(v); });
}

void HaierAC160::set_self_clean_switch(HaierAC160Switch *self_clean_sw) {
    this->self_clean_sw_ = self_clean_sw;
    this->set_switch_(self_clean_sw, "Self Clean",
        [this]() { return ac_->getClean(); },
        [this](bool v) { ac_->setClean(v); });
}

void HaierAC160::set_turbo_switch(HaierAC160Switch *turbo_sw) {
    this->turbo_sw_ = turbo_sw;
    this->set_switch_(turbo_sw, "Turbo",
        [this]() { return ac_->getTurbo(); },
        [this](bool v) { ac_->setTurbo(v); });
}

void HaierAC160::set_quiet_switch(HaierAC160Switch *quiet_sw) {
    this->quiet_sw_ = quiet_sw;
    this->set_switch_(quiet_sw, "Quiet",
        [this]() { return ac_->getQuiet(); },
        [this](bool v) { ac_->setQuiet(v); });
}

void HaierAC160::set_health_switch(HaierAC160Switch *health_sw) {
    this->health_sw_ = health_sw;
    this->set_switch_(health_sw, "Health",
        [this]() { return ac_->getHealth(); },
        [this](bool v) { ac_->setHealth(v); });
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
                ESP_LOGE(TAG, "Fan Speed '%s' is invalid", fan_speed_str.c_str());
        }
    );
}

void HaierAC160::disable_on_timer() {
    ac_->setOnTimer(0);
    this->on_timer_hour_num_ = 0;
    this->on_timer_minute_num_ = 0;
    this->on_timer_hour_se_->make_call()
        .with_index(0).perform();
    this->on_timer_minute_se_->make_call()
        .with_index(0).perform();
}

void HaierAC160::on_timer_select_handler() {
    ESP_LOGD(TAG, "On Timer wae select as %02d:%02d.",
            this->on_timer_hour_num_, this->on_timer_minute_num_);

    if ((ac_->getPower() == true) &&
        (ac_->getTimerMode() != kHaierAcYrw02OffTimer) &&
        (ac_->getTimerMode() != kHaierAcYrw02OffThenOnTimer)) {
        ESP_LOGW(TAG, "The air conditioner has been turned on.");
        this->disable_on_timer();
        return;
    }
    uint16_t total_mins = this->on_timer_hour_num_ * 60 +
        this->on_timer_minute_num_;
    ESP_LOGW(TAG, "The AC will turn on in %d minutes.",
            total_mins);

    ac_->setOnTimer(total_mins);
    this->perform(true);

    this->set_timeout("on_timer", total_mins * 60 * 1000, [this]() {
        ESP_LOGI(TAG, "The air conditioner has been turned on.");
        this->disable_on_timer();
        ac_->setPower(true);
        this->power_sw_->control(true);
    });
}

void HaierAC160::set_on_timer_hour_select(HaierAC160Select *on_timer_hour_se) {
    this->on_timer_hour_se_ = on_timer_hour_se;
    this->on_timer_hour_se_->set_callback_handler(
        [this](const std::string &hour_str) -> void {
            ESP_LOGD(TAG,
                "On Timer Hour was selected as %s", hour_str.c_str());

            if (hour_str == TIMER_OFF_STR) {
                this->on_timer_hour_num_ = 0;
            } else {
                this->on_timer_hour_num_ = std::stoi(hour_str);
                this->on_timer_select_handler();
            }
        }
    );
}

void HaierAC160::set_on_timer_minute_select(HaierAC160Select *on_timer_minute_se) {
    this->on_timer_minute_se_ = on_timer_minute_se;
    this->on_timer_minute_se_->set_callback_handler(
        [this](const std::string &min_str) -> void {
            ESP_LOGD(TAG,
                "On Timer Minute was selected as %s", min_str.c_str());

            if (min_str == TIMER_OFF_STR) {
                this->on_timer_minute_num_ = 0;
            } else {
                this->on_timer_minute_num_ = std::stoi(min_str);
                this->on_timer_select_handler();
            }
        }
    );
}

void HaierAC160::disable_off_timer() {
    ac_->setOffTimer(0);
    this->off_timer_hour_num_ = 0;
    this->off_timer_minute_num_ = 0;
    this->off_timer_hour_se_->make_call()
        .with_index(0).perform();
    this->off_timer_minute_se_->make_call()
        .with_index(0).perform();
}

void HaierAC160::off_timer_select_handler() {
    ESP_LOGD(TAG, "Off Timer wae select as %02d:%02d.",
            this->off_timer_hour_num_, this->off_timer_minute_num_);

    if ((ac_->getPower() == false) &&
        (ac_->getTimerMode() != kHaierAcYrw02OnTimer) &&
        (ac_->getTimerMode() != kHaierAcYrw02OnThenOffTimer)) {
        ESP_LOGW(TAG, "The air conditioner has been turned off.");
        this->disable_off_timer();
        return;
    }
    uint16_t total_mins = this->off_timer_hour_num_ * 60 +
        this->off_timer_minute_num_;
    ESP_LOGW(TAG, "The AC will turn off in %d minutes.",
            total_mins);

    ac_->setOffTimer(total_mins);
    this->perform(true);

    this->set_timeout("off_timer", total_mins * 60 * 1000, [this]() {
        ESP_LOGI(TAG, "The air conditioner has been turned off.");
        this->disable_off_timer();
        ac_->setPower(false);
        this->power_sw_->control(false);
    });
}

void HaierAC160::set_off_timer_hour_select(HaierAC160Select *off_timer_hour_se) {
    this->off_timer_hour_se_ = off_timer_hour_se;
    this->off_timer_hour_se_->set_callback_handler(
        [this](const std::string &hour_str) -> void {
            ESP_LOGD(TAG, "Off Timer Hour was selected as %s", hour_str.c_str());

            if (hour_str == TIMER_OFF_STR) {
                this->off_timer_hour_num_ = 0;
            } else {
                this->off_timer_hour_num_ = std::stoi(hour_str);
                this->off_timer_select_handler();
            }
        }
    );
}

void HaierAC160::set_off_timer_minute_select(HaierAC160Select *off_timer_minute_se) {
    this->off_timer_minute_se_ = off_timer_minute_se;
    this->off_timer_minute_se_->set_callback_handler(
        [this](const std::string &min_str) -> void {
            ESP_LOGD(TAG, "Off Timer Minute was selected as %s", min_str.c_str());

            if (min_str == TIMER_OFF_STR) {
                this->off_timer_minute_num_ = 0;
            } else {
                this->off_timer_minute_num_ = std::stoi(min_str);
                this->off_timer_select_handler();
            }
        }
    );
}

}  // namespace haier_ac160
}  // namespace esphome
