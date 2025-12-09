#include "haier_ac160.h"

namespace esphome {
namespace haier_ac160 {

static const char *const TAG = "climate.haier_ac160";

void ClimateIRHaierAC160::init(uint16_t pin,
        const bool recovery, const bool inverted) {
    auto climate_restore = this->restore_state_();
    if (recovery && climate_restore.has_value()) {
        ESP_LOGI(TAG, "Configuration items have been restored");
        climate_restore->apply(this);
    } else {
        ESP_LOGI(TAG,
            "The configuration was not restored because it was %s but %s.",
            recovery ? "enabled" : "disabled",
            climate_restore.has_value() ? "has a value" : "has no value");
        this->mode = climate::CLIMATE_MODE_OFF;
        this->target_temperature = HAIER_AC_DEF_TEMP;
        this->fan_mode = climate::CLIMATE_FAN_AUTO;
        this->swing_mode = climate::CLIMATE_SWING_OFF;
        this->preset = climate::CLIMATE_PRESET_NONE;
    }

    if (isnan(this->target_temperature)) {
        this->target_temperature = HAIER_AC_DEF_TEMP;
    }

    ac_ = new IRHaierAC160(pin, inverted);
    ac_->begin();
    ac_->stateReset();
    this->setup_ir_cmd();

    ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
    ESP_LOGD(TAG, "  %s\n", ac_->toString().c_str());
}

void ClimateIRHaierAC160::setup_ir_cmd() {
    uint8_t state = 0;
    if (this->mode == climate::CLIMATE_MODE_OFF) {
        if (ac_->getPower() != false) ac_->off();
    } else {
        if (ac_->getPower() != true) ac_->on();

        if (this->mode == climate::CLIMATE_MODE_COOL)
                state = kHaierAcYrw02Cool;
        else if (this->mode == climate::CLIMATE_MODE_HEAT)
                state = kHaierAcYrw02Heat;
        else if (this->mode == climate::CLIMATE_MODE_DRY)
                state = kHaierAcYrw02Dry;
        else if (this->mode == climate::CLIMATE_MODE_FAN_ONLY)
                state = kHaierAcYrw02Fan;
        else // climate::CLIMATE_MODE_AUTO
                state = kHaierAcYrw02Auto;
        if (state != ac_->getMode()) ac_->setMode(state);
  
        if (this->fan_mode == climate::CLIMATE_FAN_LOW)
                state = kHaierAcYrw02FanLow;
        else if (this->fan_mode == climate::CLIMATE_FAN_MEDIUM)
                state = kHaierAcYrw02FanMed;
        else if (this->fan_mode == climate::CLIMATE_FAN_HIGH)
                state = kHaierAcYrw02FanHigh;
        else // climate::CLIMATE_FAN_AUTO
                state = kHaierAcYrw02FanAuto;
        if (state != ac_->getFan()) {
            ac_->setQuiet(false);
            ac_->setFan(state);
        }
  
        if (this->swing_mode == climate::CLIMATE_SWING_OFF)
                state = kHaierAc160SwingVOff;
        else // climate::CLIMATE_SWING_VERTICAL
                state = kHaierAc160SwingVAuto;
        if (state != ac_->getSwingV()) ac_->setSwingV(state);
  
        if (ac_->getTemp() != (uint8_t) this->target_temperature)
            ac_->setTemp((uint8_t) this->target_temperature);

        state = this->preset_ == HaierAC160Preset::PRESET_COMFORT;
        if (state != ac_->getHealth()) ac_->setHealth(state);
        state = this->preset_ == HaierAC160Preset::PRESET_SELF_CLEAN;
        if (state != ac_->getClean()) ac_->setClean(state);
        if (this->sleep_ != ac_->getSleep())
            ac_->setSleep(this->sleep_);
        // Turbo and Quiet only availible in Heat/Cool mode
        if ((this->mode == climate::CLIMATE_MODE_COOL) ||
            (this->mode == climate::CLIMATE_MODE_HEAT)) {
            state = this->preset_ == HaierAC160Preset::PRESET_BOOST;
            if (state != ac_->getTurbo()) ac_->setTurbo(state);
            if (this->fan_mode == climate::CLIMATE_FAN_QUIET)
                ac_->setQuiet(true);
        }
    }
}

climate::ClimateTraits ClimateIRHaierAC160::traits() {
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({
        climate::CLIMATE_MODE_OFF,
        climate::CLIMATE_MODE_COOL,
        climate::CLIMATE_MODE_HEAT,
        climate::CLIMATE_MODE_FAN_ONLY,
        climate::CLIMATE_MODE_DRY,
        climate::CLIMATE_MODE_AUTO
    });
    traits.set_supported_fan_modes({
        climate::CLIMATE_FAN_AUTO,
        climate::CLIMATE_FAN_QUIET,
        climate::CLIMATE_FAN_LOW,
        climate::CLIMATE_FAN_MEDIUM,
        climate::CLIMATE_FAN_HIGH
    });
    traits.set_supported_swing_modes({
        climate::CLIMATE_SWING_OFF,
        climate::CLIMATE_SWING_VERTICAL
    });
    traits.set_supported_presets({
        climate::CLIMATE_PRESET_NONE,
        climate::CLIMATE_PRESET_COMFORT,
        climate::CLIMATE_PRESET_BOOST
    });
    if (!this->supported_custom_presets_.empty())
        traits.set_supported_custom_presets(this->supported_custom_presets_);

    traits.set_visual_max_temperature(HAIER_AC_MAX_TEMP);
    traits.set_visual_min_temperature(HAIER_AC_MIN_TEMP);
    traits.set_visual_temperature_step(1);

    return traits;
}

void ClimateIRHaierAC160::perform() {
    this->setup_ir_cmd();
    if (ac_->getPower())
        ac_->send();

    ESP_LOGD(TAG, "Haier A/C remote is in the following state:");
    ESP_LOGD(TAG, "  %s", ac_->toString().c_str());
}

void ClimateIRHaierAC160::control(const climate::ClimateCall &call) {
    if (call.get_mode().has_value())
        this->mode = *call.get_mode();

    if (call.get_target_temperature().has_value())
        this->target_temperature = *call.get_target_temperature();

    if (call.get_fan_mode().has_value())
        this->fan_mode = *call.get_fan_mode();

    if (call.get_swing_mode().has_value())
        this->swing_mode = *call.get_swing_mode();

    if (call.get_preset().has_value())
        this->preset_ = Converters::to_haier_preset(call.get_preset().value());
    else if (call.has_custom_preset())
        this->preset_ = Converters::to_haier_preset(call.get_custom_preset());

    this->perform();
    this->publish_state();
}

void ClimateIRHaierAC160::display_switch_handler(bool state) {
    ESP_LOGD(TAG, "Display switch state changed to %s",
        state ? "ON" : "OFF");
    if (state != ac_->getLightToggle()) {
        ac_->setLightToggle(state);
        if (ac_->getPower())
            ac_->send();
    }
}

void ClimateIRHaierAC160::set_display_switch(ClimateIRHaierAC160Switch *display_sw) {
    this->display_sw_ = display_sw;
    this->display_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->display_switch_handler(state);
        }
    );
}

void ClimateIRHaierAC160::aux_heating_switch_handler(bool state) {
    ESP_LOGD(TAG, "Auxiliary Heating switch state changed to %s",
        state ? "ON" : "OFF");
    if (state != ac_->getAuxHeating()) {
        ac_->setAuxHeating(state);
        if (ac_->getPower())
            ac_->send();
    }
}

void ClimateIRHaierAC160::set_aux_heating_switch(
        ClimateIRHaierAC160Switch *aux_heating_sw) {
    this->aux_heating_sw_ = aux_heating_sw;
    this->aux_heating_sw_->set_callback_handler(
        [this](bool state) -> void {
            this->aux_heating_switch_handler(state);
        }
    );
}

void ClimateIRHaierAC160::swing_mode_select_handler(
        HaierAC160SwingMode swing_mode) {
    ESP_LOGD(TAG, "Swing Mode was selected as %s",
        get_swing_mode_str(swing_mode));
}

void ClimateIRHaierAC160::set_swing_mode_select(
        ClimateIRHaierAC160Select<HaierAC160SwingMode> *swing_mode_se) {
    this->swing_mode_se_ = swing_mode_se;
    this->swing_mode_se_->set_callback_handler(
        [this](HaierAC160SwingMode swing_mode) -> void {
            this->swing_mode_select_handler(swing_mode);
        }
    );
}

}  // namespace haier_ac160
}  // namespace esphome
