#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include "globals.h"
#include "hal/board_config.h"
#include "hal/power_manager.h"
#include "hal/input_manager.h"
#include "system/clock_logic.h"
#include "system/ble_manager.h"
#include "system/button_manager.h"
#include "system/logger.h"
#include "system/step_counter.h"
#include "ui/display.h"
#include "apps/UI_Manager.h"

extern void InputManager::buttonISR();

SemaphoreHandle_t xDisplayMutex = NULL;
QueueHandle_t xButtonQueue = NULL;
QueueHandle_t xUICommandQueue = NULL;

void buttonTask(void *pvParameters);

void attachButtonISR();

void setup()
{
  Serial.begin(115200);

  // Logger must be first so all subsequent inits can use it
  Logger::init();

  PowerManager::initWatchdog();
  xDisplayMutex = xSemaphoreCreateMutex();
  if (xDisplayMutex == NULL)
  {
    LOG_E("main", "Failed to create display mutex");
    vTaskDelete(NULL);
  }

  PowerManager::initPowerManagement();
  PowerManager::initBattery();
  DisplayDriver::initDisplay();
  BLEManager::init();
  ClockLogic::initClock();
  InputManager::init();
  StepCounter::run();

  xButtonQueue = xQueueCreate(10, sizeof(uint8_t));
  xUICommandQueue = xQueueCreate(5, sizeof(uint8_t));

  LOG_I("main", "Setup complete");
  UIManager::setState(UIManager::CLOCK);
  UIManager::run();
  ButtonManager::run();
}

void loop()
{
  PowerManager::feedWatchdog();
  vTaskDelay(pdMS_TO_TICKS(1000));
}
