#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include "apps/ui_manager.h"
#include "ui/display.h"
#include "globals.h"

namespace UI_Manager
{
    WATCH_STATE currentState;

    void setState(WATCH_STATE state)
    {
        currentState = state;
    }

    WATCH_STATE getState()
    {
        return currentState;
    }

    void incrementState()
    {
        if (currentState < WATCH_STATE::COUNT)
        {
            currentState = static_cast<WATCH_STATE>(static_cast<int>(currentState) + 1);
        }
    }

    void decrementState()
    {
        if (currentState > WATCH_STATE::CLOCK)
        {
            currentState = static_cast<WATCH_STATE>(static_cast<int>(currentState) - 1);
        }
    }

    void UITask(void *pvParameters)
    {

        (void)pvParameters;
        for (;;)
        {

            switch (getState())
            {
            case CLOCK:
                if (xSemaphoreTake(xDisplayMutex, portMAX_DELAY) == pdTRUE)
                {
                    Display::drawClock();
                    Serial.println("ClockTask: Tick");
                }
                break;

            case TIMER:
                if (xSemaphoreTake(xDisplayMutex, portMAX_DELAY) == pdTRUE)
                {
                    Display::drawTimer();
                    Serial.println("TimerTask: Tick");
                }
                break;

            case STOPWATCH:
                if (xSemaphoreTake(xDisplayMutex, portMAX_DELAY) == pdTRUE)
                {
                    Display::drawStopwatch();
                    Serial.println("StopwatchTask: Tick");
                }
                break;

            // Just so the compiler doesn't complain, it should never reach here
            case COUNT:
                break;
            }

            xSemaphoreGive(xDisplayMutex);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
    void run()
    {
        Serial.println("UI Manager Starting...");
        xTaskCreate(UITask, "UITask", 2048, NULL, 1, NULL);
    }
}
