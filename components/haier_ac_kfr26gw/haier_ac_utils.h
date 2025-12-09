#pragma once

#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"

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

template<typename EnumT>
class ClimateIRHaierAC160Select : public select::Select, public Component {
    public:
        void set_callback_handler(std::function<void(EnumT)> cb)
            { this->cb_ = cb; }

    protected:
        std::function<void(EnumT)> cb_;

        void control(size_t index) override {
           this->publish_state(index);
           if (this->cb_) this->cb_(static_cast<EnumT>(index));
        };
};

} // namespace esphome
} // namespace haier_ac160
