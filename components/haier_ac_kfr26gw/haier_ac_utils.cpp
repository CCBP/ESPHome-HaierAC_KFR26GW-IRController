#include "haier_ac_utils.h"

namespace esphome {
namespace haier_ac160 {

void ClimateIRHaierAC160Switch::write_state(bool state) {
    this->cb_(state);
    // Report state back
    publish_state(state);
}

} // namespace esphome
} // namespace haier_ac_160
