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
  Serial.begin(9600);

  xDisplayMutex = xSemaphoreCreateMutex();
  if (xDisplayMutex == NULL)
  {
    Serial.println("Failed to create display mutex");
    vTaskDelete(NULL);
  }

  // Initialize power manager
  PowerManager::initPowerManagement();

  // Initialize display
  DisplayDriver::initDisplay();

  BLEManager::init();
  // Initialize clock logic
  ClockLogic::initClock();

  InputManager::init();

  xButtonQueue = xQueueCreate(10, sizeof(uint8_t));
  xUICommandQueue = xQueueCreate(5, sizeof(uint8_t));

  attachButtonISR();

  Serial.println("Setup complete.");
  UI_Manager::setState(UI_Manager::CLOCK);
  UI_Manager::run();

  xTaskCreate(buttonTask, "ButtonTask", 2048, NULL, 2, NULL);
}

void buttonTask(void *pvParameters)
{
  uint8_t receivedBtnID;
  for (;;)
  {
    if (xQueueReceive(xButtonQueue, &receivedBtnID, portMAX_DELAY) == pdPASS)
    {
      uint8_t cmd = 0;

      // Logic: Decide what the button does based on current screen
      if (receivedBtnID == BTN_ID_A)
      {
        cmd = UI_CMD_NEXT;
      }
      else if (receivedBtnID == BTN_ID_B)
      {
        cmd = UI_CMD_PREV;
      }
      else if (receivedBtnID == BTN_ID_C)
      {
        // Only send stopwatch command if we are actually ON the stopwatch screen
        if (UI_Manager::getState() == UI_Manager::STOPWATCH)
        {
          cmd = UI_CMD_STOPWATCH_TOGGLE;
        }
      }
      else if (receivedBtnID == BTN_ID_D)
      {
        if (UI_Manager::getState() == UI_Manager::STOPWATCH)
        {
          cmd = UI_CMD_STOPWATCH_RESET;
        }
      }

      // Send to UI Task if a valid command was generated
      if (cmd != 0)
      {
        xQueueSend(xUICommandQueue, &cmd, 0);
      }

      // Debounce delay
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }
}

void attachButtonISR()
{
  attachInterrupt(digitalPinToInterrupt(BUTTON_A), InputManager::buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_B), InputManager::buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_C), InputManager::buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_D), InputManager::buttonISR, FALLING);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  BLEManager::parseUartTime();
}
