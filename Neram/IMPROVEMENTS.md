# Project Neram: Improvement Plan

## 1. Button Navigation Redesign

Current state: Button A = NEXT, B = PREV, C = STOPWATCH_TOGGLE, D = STOPWATCH_RESET.  
Target: Physical layout with two left buttons (Select, Back) and two right buttons (Up, Down) enabling hierarchical menu navigation.

- [ ] Remap `board_config.h` pin assignments with clear physical-layout names:
  - `BUTTON_LEFT_SELECT` (e.g. D2 — left top)
  - `BUTTON_LEFT_BACK` (e.g. D3 — left bottom)
  - `BUTTON_RIGHT_UP` (e.g. D0 — right top)
  - `BUTTON_RIGHT_DOWN` (e.g. D1 — right bottom)
- [ ] Update `input_manager.cpp` ISR and `ButtonManager::buttonTask` to use the new names
- [ ] Add new UI commands to `globals.h`:
  - `UI_CMD_UP`, `UI_CMD_DOWN`, `UI_CMD_SELECT`, `UI_CMD_BACK`
  - Remove `UI_CMD_NEXT`, `UI_CMD_PREV`, `UI_CMD_STOPWATCH_TOGGLE`, `UI_CMD_STOPWATCH_RESET`
- [ ] Rewrite `button_manager.cpp` mapping:
  - Right-Up → `UI_CMD_UP`
  - Right-Down → `UI_CMD_DOWN`
  - Left-Select → `UI_CMD_SELECT`
  - Left-Back → `UI_CMD_BACK`
- [ ] Add long-press detection in `ButtonManager` (hold threshold ~600 ms) for power or contextual actions

---

## 2. Hierarchical UI State Machine

Current state: flat cycle between CLOCK → TIMER → STOPWATCH with wrap-around.  
Target: root menu layer that can push/pop into sub-screens using a navigation stack.

- [ ] Define a `NavStack` (fixed-depth array, e.g. depth 4) in `ui_manager.h` to track the current screen hierarchy
- [ ] Define all screen IDs in an enum:
  ```
  SCREEN_CLOCK, SCREEN_MAIN_MENU,
  SCREEN_STOPWATCH, SCREEN_TIMER_SET, SCREEN_TIMER_RUN,
  SCREEN_SETTINGS, SCREEN_SETTINGS_BLE, SCREEN_SETTINGS_DISPLAY
  ```
- [ ] On `UI_CMD_SELECT`: push the highlighted menu item's screen onto the stack and render it
- [ ] On `UI_CMD_BACK`: pop the stack and return to the parent screen; from root, do nothing (or sleep)
- [ ] On `UI_CMD_UP` / `UI_CMD_DOWN`: move a cursor index within the current screen's item list
- [ ] Remove the old `incrementState()` / `decrementState()` helpers

---

## 3. Main Menu Screen

There is currently no main menu — navigation is implicit key cycling.

- [ ] Create a `MenuScreen` struct holding an array of `MenuItem` (label string + target screen ID)
- [ ] Implement `Display::drawMenu(items[], count, cursorIndex)` in `display.cpp` using Adafruit GFX list rendering
- [ ] Add a top-level menu with at least: Clock, Stopwatch, Timer, Settings
- [ ] Highlight the selected item (inverted rectangle or `>` prefix)
- [ ] Entering CLOCK from the menu should go directly to the clock face, not back through the menu on Back (push SCREEN_CLOCK on top of SCREEN_MAIN_MENU)

---

## 4. Timer Feature (currently a placeholder)

`Display::drawTimer` prints only `"Timer:"` — the timer itself is not implemented.

- [ ] Add timer state to `ClockLogic`: `timerDurationSeconds`, `timerRemainingSeconds`, `timerRunning` flag
- [ ] Implement `ClockLogic::setTimer(uint32_t seconds)`, `startTimer()`, `stopTimer()`, `resetTimer()`
- [ ] Decrement `timerRemainingSeconds` inside the existing clock task (1 Hz tick)
- [ ] Add a timer-set screen (`SCREEN_TIMER_SET`): Up/Down adjust minutes, Select confirms and pushes `SCREEN_TIMER_RUN`
- [ ] Implement `Display::drawTimerSet(minutes, seconds)` and `Display::drawTimerRun(remaining)` in `display.cpp`
- [ ] Trigger a visual alert (invert display or blink) when timer reaches zero
- [ ] Back from `SCREEN_TIMER_RUN` stops and resets the timer

---

## 5. Status Bar

There is no persistent status bar — connection state and battery level are invisible to the user.

- [ ] Reserve the top 12 px of the 128×128 display as a status bar drawn by every screen
- [ ] Implement `Display::drawStatusBar(bool bleConnected, uint8_t batteryPct)` in `display.cpp`
- [ ] Expose `BLEManager::isConnected()` (already have a `connected` bool in `ble_manager.cpp`) via a public getter
- [ ] Expose `PowerManager::getBatteryPercent()` — add SAADC read on P0.02 mapped to 0–100%
- [ ] Call `drawStatusBar` at the top of `UIManager::drawViewAt` before delegating to the active screen

---

## 6. BLE Notifications Display

ANCS notifications are logged to Serial only — the user never sees them on screen.

- [ ] Add a circular buffer (e.g. 4 entries) of `AncsNotification { char title[32]; char app[16]; }` in `ble_manager.h`
- [ ] Populate the buffer inside `ancs_notification_callback` instead of (or in addition to) Serial output
- [ ] Add a `SCREEN_NOTIFICATIONS` screen and a "Notifications" entry in the main menu
- [ ] Implement `Display::drawNotificationList(AncsNotification[], count, cursor)` in `display.cpp`
- [ ] Show a dot or badge indicator in the status bar when unread notifications exist

---

## 7. Input Manager: ISR Robustness

A single shared ISR reads `digitalRead` on all pins — prone to ghost presses and pin-level races on nRF52840.

- [ ] Assign a separate ISR per button (or use nRF52 GPIOTE individual channels) so only the triggering pin is polled
- [ ] Move `xQueueSendFromISR` with proper `xHigherPriorityTaskWoken` and `portYIELD_FROM_ISR` into each ISR
- [ ] Increase debounce window from 200 ms to 50 ms hardware + 150 ms software (ISR timestamps via `xTaskGetTickCountFromISR`)
- [ ] Emit `BTN_EVENT_PRESS` and `BTN_EVENT_RELEASE` so `ButtonManager` can measure hold duration for long-press

---

## 8. Watchdog Feeding Location

`PowerManager::feedWatchdog()` is called in `loop()` — if the UI or BLE task hangs, the watchdog still gets fed.

- [ ] Move watchdog feeding into the highest-priority periodic task (ClockTask, priority 4) or a dedicated watchdog task
- [ ] Remove the `feedWatchdog` call from `loop()`; remove the Serial uptime print from `loop()` (use a debug build flag)
- [ ] Consider per-task "alive" flags that the watchdog task checks before feeding, so a hung task triggers a reset

---

## 9. Display Mutex Usage

`UITask` takes `xDisplayMutex` around every full redraw — this is correct but the mutex is never taken in `display.cpp` helpers, creating a hidden contract.

- [ ] Document in `display.h` that callers must hold `xDisplayMutex` before calling any `Display::` function
- [ ] Assert (`configASSERT`) inside each `Display::` function that the calling task holds the mutex (debug builds only)
- [ ] Audit all `Display::` call sites to confirm the mutex is always held

---

## 10. Memory & Stack Sizes

Stack sizes are fixed constants — no evidence they were profiled.

- [ ] Enable `uxTaskGetStackHighWaterMark` reporting for each task in a debug build, printed over Serial
- [ ] Right-size stacks based on actual high-water measurements (UITask at 4096 and ButtonTask at 2048 are likely oversized)
- [ ] Move all task stacks to statically allocated arrays (`StaticTask_t` + `StackType_t[]`) to catch stack overflows at link time and avoid heap fragmentation

---

## 12. Serial Logger Task (DONE)

All `Serial.print*` calls replaced with a queue-backed logger task (`src/system/logger.h/.cpp`).

- [x] `LogMessage_t` struct: `tick` (ms), `level`, `tag[12]`, `msg[60]`
- [x] `xLogQueue` (32-deep) created in `Logger::init()` before any other init
- [x] `vLogTask` at priority 1 — drains queue and writes formatted lines to Serial
- [x] `Logger::log()` is non-blocking; drops messages silently if queue full
- [x] Convenience macros: `LOG_D / LOG_I / LOG_W / LOG_E`
- [x] Every UI state transition logged in `setState()`, `incrementState()`, `decrementState()`
- [x] BLE connect / disconnect / secured / ANCS notification / CTS adjust all logged
- [x] Button press (DEBUG) and media command (INFO) logged in `ButtonManager`
- [x] Stopwatch start / stop (with elapsed time) / reset logged in `ClockLogic`
- [x] Serial baud raised to 115200

Output format: `[tick_ms][L][TAG] message`

---

## 13. Step Counter (IMU Pedometer)

The XIAO nRF52840 Sense has an onboard **LSM6DS3TR-C** (6-axis IMU) with a built-in hardware pedometer — the chip counts steps autonomously in silicon, no software peak-detection needed. Steps survive between I2C reads and the IMU can fire an interrupt on each detected step.

### Hardware context

- IMU: LSM6DS3TR-C on the internal I2C bus (`Wire1`, not `Wire`)
- I2C address: `0x6A`
- Library: `Seeed Arduino LSM6DS3` (add to `lib_deps` in `platformio.ini`)
- Step counter output: registers `STEP_COUNTER_L` (0x4B) + `STEP_COUNTER_H` (0x4C) → 16-bit count
- Step detector interrupt: routable to `INT1` pin for zero-CPU wake-on-step

### Tasks

- [ ] Add `Seeed_Arduino_LSM6DS3` to `lib_deps` in `platformio.ini`
- [ ] Create `src/system/step_counter.h` / `step_counter.cpp`:
  - `StepCounter::init()` — configure LSM6DS3TR-C pedometer mode:
    1. Set `FUNC_CFG_ACCESS` (0x01) bit 0 to enable embedded functions
    2. Write `PEDO_DEB_REG` for debounce (threshold 6, time 1) to reduce false positives
    3. Set `TAP_CFG` (0x58) bit 6 (`PEDO_EN`) and bit 7 (`TIMER_EN`)
    4. Set accel ODR to 26 Hz (`CTRL1_XL` = 0x20) and full-scale ±2 g — optimal for walking
    5. Reset step counter via `CTRL10_C` (0x19) bit 1 (`PEDO_RST_STEP`)
  - `StepCounter::getSteps()` → `uint16_t` — reads `STEP_COUNTER_L/H` over `Wire1`
  - `StepCounter::reset()` — pulses `PEDO_RST_STEP` then clears daily baseline
  - Internal `uint16_t dailyBaseline` — subtracted from raw count so `getSteps()` returns today's steps
- [ ] Add a `vStepTask` (priority 2, stack 1024) that polls `getSteps()` every 5 s and logs changes:
  ```
  [I][STEP] steps: 1234 (+3)
  ```
  Five-second polling avoids hammering I2C; the IMU accumulates steps between reads.
- [ ] Add `SCREEN_STEPS` to `UIManager::WATCH_STATE` enum and `stateName()` helper
- [ ] Implement `Display::drawSteps(int x, int y, uint16_t steps)` in `display.cpp`:
  - Large step count centred on screen
  - Second line: progress bar or fraction toward a fixed 10 000-step daily goal
- [ ] Wire `SCREEN_STEPS` into `UIManager::drawViewAt()` and the navigation cycle
- [ ] Expose `StepCounter::run()` and call it from `setup()` after `Logger::init()`
- [ ] Log step counter init result (success / I2C error) via `LOG_I("STEP", ...)`
- [ ] Daily reset: in `ClockLogic::clockTask`, when `tm_hour == 0 && tm_min == 0 && tm_sec == 0`, call `StepCounter::reset()` and log `[I][STEP] daily reset`

---

## 11. Code Organization: Remove Arduino-style `loop()` Dependency

`loop()` running as an implicit FreeRTOS task conflates Arduino lifecycle with explicit task management.

- [ ] Convert `loop()` to an explicit `vMainTask(void*)` created with `xTaskCreate` in `setup()` so priority and stack are explicit
- [ ] Remove the `Serial.println` uptime log behind a `#ifdef DEBUG` guard so release builds are silent
- [ ] Consolidate all `xTaskCreate` calls into a single `vCreateSystemTasks()` function for clarity
