#pragma once

#include "esphome/components/switch/switch.h"

namespace esphome {
namespace haier_ac160 {

class ClimateIRHaierAC160Switch : public switch_::Switch, public Component {
    public:
        void set_callback_handler(std::function<void(bool)> cb)
            { this->cb_ = cb; }

    protected:
        std::function<void(bool)> cb_;

        void write_state(bool state) override;
};

} // namespace esphome
} // namespace haier_ac160
