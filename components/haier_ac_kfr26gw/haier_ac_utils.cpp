#include "haier_ac_utils.h"

namespace esphome {
namespace haier_ac160 {

void ClimateIRHaierAC160Switch::write_state(bool state) {
    // Report state back
    this->publish_state(state);
    if (this->cb_) this->cb_(state);
}

} // namespace esphome
} // namespace haier_ac_160
