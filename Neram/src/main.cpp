#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

#include "hal/board_config.h"
#include "hal/power_manager.h"
#include "system/clock_logic.h"
#include "system/ble_manager.h"
#include "ui/display.h"
#include "apps/clock.h"

void setup()
{
  Serial.begin(9600);

  // Initialize power manager
  PowerManager::initPowerManagement();

  // Initialize display
  DisplayDriver::initDisplay();

  BLEManager::init();
  // Initialize clock logic
  ClockLogic::initClock();

  Serial.println("Setup complete.");

  Clock::run();
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  BLEManager::parseUartTime();
}