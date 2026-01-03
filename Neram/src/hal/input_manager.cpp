#include <Arduino.h>
#include "hal/input_manager.h"
#include "board_config.h"
#include "globals.h"

namespace InputManager
{
    void init()
    {
        pinMode(BUTTON_A, INPUT_PULLUP);
        pinMode(BUTTON_B, INPUT_PULLUP);
        pinMode(BUTTON_C, INPUT_PULLUP);
        pinMode(BUTTON_D, INPUT_PULLUP);
    }

    extern void buttonISR()
    {
        uint32_t currentTime = millis();

        static uint32_t lastInterruptTime = 0;
        const uint32_t debounceThreshold = 200;

        if (currentTime - lastInterruptTime > debounceThreshold)
        {
            uint8_t currentButtonState = 0;

            // Check which button was actually pressed
            if (digitalRead(BUTTON_A) == LOW)
                currentButtonState = BTN_ID_A;
            else if (digitalRead(BUTTON_B) == LOW)
                currentButtonState = BTN_ID_B;
            else if (digitalRead(BUTTON_C) == LOW)
                currentButtonState = BTN_ID_C;
            else if (digitalRead(BUTTON_D) == LOW)
                currentButtonState = BTN_ID_D;

            if (currentButtonState != 0)
            {
                lastInterruptTime = currentTime;

                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                xQueueSendFromISR(xButtonQueue, &currentButtonState, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
    }

}
