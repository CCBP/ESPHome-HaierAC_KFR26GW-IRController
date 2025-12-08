#pragma once

#include "esphome/core/log.h"
#include "esphome/components/climate/climate.h"

#include "haier_ac_adapter.h"
#include "haier_ac_utils.h"
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
        void set_display_switch(ClimateIRHaierAC160Switch *display_sw);
        void set_aux_heating_switch(ClimateIRHaierAC160Switch *aux_heating);

    protected:
        IRHaierAC160 *ac_{nullptr};
        std::vector<const char *> supported_custom_presets_{};
        HaierAC160Preset preset_{HaierAC160Preset::PRESET_NONE};
        bool sleep_{false};

        ClimateIRHaierAC160Switch *display_sw_{nullptr};
        ClimateIRHaierAC160Switch *aux_heating_sw_{nullptr};

        void display_switch_handler(bool state);
        void aux_heating_switch_handler(bool state);

        void setup_ir_cmd();

        climate::ClimateTraits traits() override;
        void control(const climate::ClimateCall &call) override;

};

}  // namespace haier_ac160
}  // namespace esphome
