#pragma once

#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace haier_ac160 {

class HaierAC160Number : public number::Number, public Component {
    public:
        void set_callback_handler(std::function<void(float)> cb)
            { this->cb_ = cb; }

    protected:
        std::function<void(float)> cb_;

        void control(float value) override;
};

class HaierAC160Switch : public switch_::Switch, public Component {
    public:
        void set_callback_handler(std::function<void(bool)> cb)
            { this->cb_ = cb; }

    protected:
        std::function<void(bool)> cb_;

        void write_state(bool state) override;
};

class HaierAC160Select : public select::Select, public Component {
    public:
        void set_callback_handler(std::function<void(std::string)> cb)
            { this->cb_ = cb; }

    protected:
        std::function<void(std::string)> cb_;

        void control(const std::string &value) override;
};

} // namespace esphome
} // namespace haier_ac160
