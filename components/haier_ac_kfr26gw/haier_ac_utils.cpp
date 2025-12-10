#include "haier_ac_utils.h"

namespace esphome {
namespace haier_ac160 {

void HaierAC160Number::control(float value) {
    this->publish_state(value);
    if (this->cb_) this->cb_(value);
}

void HaierAC160Switch::write_state(bool state) {
    this->publish_state(state);
    if (this->cb_) this->cb_(state);
}

} // namespace esphome
} // namespace haier_ac_160
