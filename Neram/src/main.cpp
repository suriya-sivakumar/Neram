#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

#include "hal/board_config.h"
#include "hal/power_manager.h"
#include "system/clock_logic.h"
#include "ui/display.h"

void setup()
{
  Serial.begin(9600);

  // Initialize power manager
  PowerManager::initPowerManagement();

  // Initialize display
  DisplayDriver::initDisplay();

  // Initialize clock logic
  ClockLogic::initClock();

  Serial.println("Setup complete.");
}

void loop()
{

  while (1)
  {
    Display::drawClock();
    Serial.println("Clock updated on display.");
    delay(500);
  }
}