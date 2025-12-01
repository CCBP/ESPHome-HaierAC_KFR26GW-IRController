#pragma once

#include "esphome/core/log.h"
#include "esphome/components/climate/climate.h"

#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Haier.h"

namespace esphome {
namespace haier_ac_kfr26gw {

    class ClimateIRHaierAC160 : public climate::Climate {
        public:
            ClimateIRHaierAC160() : climate::Climate() {}

            void init(uint16_t pin, const bool inverted = false);

        protected:
            IRHaierAC160 *ac_{nullptr};
            void setup_ir_cmd();

            climate::ClimateTraits traits() override;
            void control(const climate::ClimateCall &call) override;

    };

}  // namespace haier_ac_kfr26gw
}  // namespace esphome
