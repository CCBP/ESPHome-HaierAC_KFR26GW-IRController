#pragma once

#include "esphome/core/log.h"
#include "esphome/components/climate/climate.h"

#include "haier_ac_adapter.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Haier.h"

namespace esphome {
namespace haier_ac160 {

class ClimateIRHaierAC160 : public climate::Climate {
    public:
        ClimateIRHaierAC160() : climate::Climate() {}

        void init(uint16_t pin, const bool recovery = true,
            const bool inverted = false);

        void perform();

        void set_custom_presets(std::initializer_list<const char *> presets)
            { this->supported_custom_presets_ = presets; }
        void set_light_toggle(const bool state) { this->light_toggle_ = state; }
        void set_aux_heating(const bool state) { this->aux_heating_ = state; }

    protected:
        IRHaierAC160 *ac_{nullptr};
        std::vector<const char *> supported_custom_presets_{};
        HaierAC160Preset preset_{HaierAC160Preset::PRESET_NONE};
        bool light_toggle_{true};
        bool aux_heating_{false};
        bool sleep_{false};

        void setup_ir_cmd();

        climate::ClimateTraits traits() override;
        void control(const climate::ClimateCall &call) override;

};

}  // namespace haier_ac160
}  // namespace esphome
