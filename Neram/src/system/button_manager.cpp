#include <Arduino.h>
#include <FreeRTOS.h>

#include "globals.h"
#include "hal/board_config.h"

#include "apps/UI_Manager.h"
#include "system/ble_manager.h"
#include "hal/input_manager.h"

namespace ButtonManager
{

    void buttonTask(void *pvParameters)
    {
        uint8_t receivedBtnID;
        for (;;)
        {
            if (xQueueReceive(xButtonQueue, &receivedBtnID, portMAX_DELAY) == pdPASS)
            {
                uint8_t cmd = 0;

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
                    if (UIManager::getState() == UIManager::STOPWATCH)
                    {
                        cmd = UI_CMD_STOPWATCH_TOGGLE;
                    }
                    else if (UIManager::getState() == UIManager::CLOCK)
                    {
                        // From clock screen, C button sends Play/Pause media command
                        BLEManager::sendMediaControl(0xCD);
                    }
                }
                else if (receivedBtnID == BTN_ID_D)
                {
                    if (UIManager::getState() == UIManager::STOPWATCH)
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

    void run()
    {
        attachButtonISR();
        xTaskCreate(buttonTask, "ButtonTask", 2048, NULL, 2, NULL);
    }
}
