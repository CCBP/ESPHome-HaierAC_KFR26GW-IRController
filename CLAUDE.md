# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

ESPHome custom component for controlling **Haier KFR-26GW series** air conditioners via infrared, using the `IRHaierAC160` protocol from [IRremoteESP8266](https://github.com/crankyoldgit/IRremoteESP8266) (v2.8.6 pinned). The component is consumed as an `external_components` source from GitHub.

**Design choice:** Does NOT use ESPHome's `climate` component — the Haier IR protocol fields don't map 1:1 to climate's abstraction. Instead, control is exposed via individual `number`, `switch`, and `select` ESPHome entities.

## Build & Flash

```shell
esphome run example.yaml          # compile + flash (needs secrets.yaml alongside)
esphome logs example.yaml         # view serial output
esphome compile example.yaml      # compile only
esphome clean example.yaml       # clean build artifacts
```

There is no test suite, linter, or CI pipeline in this project.

## Architecture

All custom component source lives under `components/haier_ac_kfr26gw/`:

| File | Role |
|---|---|
| `__init__.py` | ESPHome Python automation: config schema, code generation, YAML → C++ wiring. Declares all sub-entities (1 number, 9 switches, 7 selects). |
| `haier_ac160.h` / `.cpp` | Core controller class `HaierAC160`. Owns the `IRHaierAC160` instance via `std::unique_ptr`, manages state restore via `ESPPreferenceObject`, and orchestrates IR transmission. Uses `set_switch_()` helper to register switch callbacks with getter/setter lambdas. |
| `haier_ac_adapter.h` / `.cpp` | Thin ESPHome entity adapters (`HaierAC160Number`, `HaierAC160Switch`, `HaierAC160Select`). Override `control()`/`write_state()` to publish state then invoke a user-land callback. |
| `haier_ac_utils.h` / `.cpp` | Enums (`HaierAC160OperateMode`, `HaierAC160SwingMode`, `HaierAC160FanSpeed`) mapped to IRremoteESP8266 constants, plus `Converts` utility for bidirectional enum↔string lookup. Wrapper methods are inlined in the header; `.cpp` only contains static data definitions. |

### Data flow

1. User changes entity in Home Assistant → ESPHome calls `control()`/`write_state()` on adapter
2. Adapter publishes new state, invokes callback on `HaierAC160`
3. `HaierAC160` compares with current `IRHaierAC160` state; if changed, calls `perform()`
4. `perform()` sends IR signal via `ac_->send()`, then persists state to flash (`rtc_.save()`)

### State persistence

Uses ESPHome's `ESPPreferenceObject` with a custom `RESTORE_STATE_VERSION` hash. The raw protocol bytes (`HaierAC160ProtocolRestore`) are serialized to flash. When `CONF_RESTORE_STATE` is true, the component restores the last known IR state on boot.

## Key Conventions

- **Namespace:** All C++ code lives in `esphome::haier_ac160`
- **Dependency:** IRremoteESP8266 types are used directly (e.g., `kHaierAcYrw02Auto`, `IRHaierAC160`) — enums in `haier_ac_utils.h` are thin wrappers over these constants
- **Switch registration:** Non-power switches use `set_switch_()` with getter/setter lambdas; power switch has its own handler due to timer-disable side effects
- **Display switch:** Maps to `IRHaierAC160::getLightToggle()/setLightToggle()` — controls the AC display on/off
- **All features are enabled by default** in the YAML config — there's a known TODO to make non-core features opt-in
- **Timer logic** uses `select` entities for hour/minute with a `"--"` sentinel for "off"; `set_timeout()` is used for the actual countdown on the ESP. Timer handlers include nullptr checks and treat `stoi("00")` == 0 as cancellation

## Known Issues (from README TODO)

- Display control and swing mode settings behave inconsistently — suspected IRremoteESP8266 protocol implementation issue
- Timer UX is poor — should consider `datetime` component
- Non-core features should be opt-in via YAML config
