#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include "globals.h"
#include "hal/board_config.h"
#include "hal/power_manager.h"
#include "system/clock_logic.h"
#include "system/ble_manager.h"
#include "ui/display.h"
#include "apps/UI_Manager.h"

volatile uint32_t last_interrupt_time = 0;

SemaphoreHandle_t xDisplayMutex = NULL;
QueueHandle_t xButtonQueue = NULL;

void buttonTask(void *pvParameters);

void buttonISR();

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

  xButtonQueue = xQueueCreate(10, sizeof(uint8_t));

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BUTTON_A), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_B), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_C), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_D), buttonISR, FALLING);

  Serial.println("Setup complete.");
  UI_Manager::setState(UI_Manager::CLOCK);
  UI_Manager::run();
  xTaskCreate(buttonTask, "ButtonTask", 2048, NULL, 1, NULL);
}

void buttonTask(void *pvParameters)
{
  uint8_t receivedBtnID;

  for (;;)
  {
    if (xQueueReceive(xButtonQueue, &receivedBtnID, portMAX_DELAY) == pdPASS)
    {
      Serial.print("Received Button: ");
      Serial.println(receivedBtnID);

      if (xSemaphoreTake(xDisplayMutex, portMAX_DELAY) == pdTRUE)
      {
        if (receivedBtnID == BTN_ID_A)
        {
          UI_Manager::incrementState();
        }
        else if (receivedBtnID == BTN_ID_B)
        {
          UI_Manager::decrementState();
        }
        xSemaphoreGive(xDisplayMutex);
      }

      vTaskDelay(pdMS_TO_TICKS(50));
    }
  }
}
void buttonISR()
{
  uint32_t interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) // Debounce time
  {
    last_interrupt_time = interrupt_time;
    uint8_t pinClicked = 0;

    if (digitalRead(BUTTON_A) == LOW)
      pinClicked = BTN_ID_A;
    else if (digitalRead(BUTTON_B) == LOW)
      pinClicked = BTN_ID_B;
    else if (digitalRead(BUTTON_C) == LOW)
      pinClicked = BTN_ID_C;
    else if (digitalRead(BUTTON_D) == LOW)
      pinClicked = BTN_ID_D;

    if (pinClicked != 0)
    {

      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      xQueueSendFromISR(xButtonQueue, &pinClicked, &xHigherPriorityTaskWoken);
      portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
  }
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  BLEManager::parseUartTime();
}
