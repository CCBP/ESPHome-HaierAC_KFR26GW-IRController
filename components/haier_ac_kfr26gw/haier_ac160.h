#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "haier_ac_adapter.h"
#include "haier_ac_utils.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "ir_Haier.h"

namespace esphome {
namespace haier_ac160 {

class HaierAC160 : public Component {
    public:
        HaierAC160() {}

        void init(uint16_t pin, const bool recovery = true,
            const bool inverted = false);

        void perform();

        void set_temperature_number(HaierAC160Number *temperature_nu);

        void set_display_switch(HaierAC160Switch *display_sw);
        void set_aux_heating_switch(HaierAC160Switch *aux_heating);

        void set_swing_mode_select(
                HaierAC160Select<HaierAC160SwingMode> *swing_mode_se);
        void set_timer_hour_select(
                HaierAC160Select<uint8_t> *timer_hour_se);
        void set_timer_minute_select(
                HaierAC160Select<uint8_t> *timer_minute_se);

    protected:
        IRHaierAC160 *ac_{nullptr};
        std::vector<const char *> supported_custom_presets_{};
        bool sleep_{false};

        HaierAC160Number *temperature_nu_{nullptr};

        HaierAC160Switch *display_sw_{nullptr};
        HaierAC160Switch *aux_heating_sw_{nullptr};

        HaierAC160Select<HaierAC160SwingMode> *swing_mode_se_{nullptr};
        HaierAC160Select<uint8_t> *timer_hour_se_{nullptr};
        HaierAC160Select<uint8_t> *timer_minute_se_{nullptr};

        void temperature_number_handler(uint8_t temp);

        void display_switch_handler(bool state);
        void aux_heating_switch_handler(bool state);

        void swing_mode_select_handler(HaierAC160SwingMode swing_mode);
        void timer_hour_select_handler(uint8_t hour);
        void timer_minute_select_handler(uint8_t minute);
};

}  // namespace haier_ac160
}  // namespace esphome
