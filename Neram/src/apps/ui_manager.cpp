#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include "apps/ui_manager.h"
#include "ui/display.h"
#include "globals.h"
#include "system/clock_logic.h"

namespace UI_Manager
{
    WATCH_STATE currentState = WATCH_STATE::CLOCK;

    void setState(WATCH_STATE state) { currentState = state; }
    WATCH_STATE getState() { return currentState; }

    void incrementState()
    {
        int next = static_cast<int>(currentState) + 1;
        if (next >= static_cast<int>(WATCH_STATE::COUNT))
        {
            currentState = WATCH_STATE::CLOCK;
        }
        else
        {
            currentState = static_cast<WATCH_STATE>(next);
        }
    }

    void decrementState()
    {
        int next = static_cast<int>(currentState) - 1;
        if (next < 0)
        {
            currentState = static_cast<WATCH_STATE>(static_cast<int>(WATCH_STATE::COUNT) - 1);
        }
        else
        {
            currentState = static_cast<WATCH_STATE>(next);
        }
    }

    void drawViewAt(WATCH_STATE state, int x)
    {
        switch (state)
        {
        case WATCH_STATE::CLOCK:
        {
            Display::drawClock(x, 50);
            break;
        }
        case WATCH_STATE::TIMER:
        {
            Display::drawTimer(x, 50);
            break;
        }
        case WATCH_STATE::STOPWATCH:
        {
            Display::drawStopwatch(x, 50,
                                   ClockLogic::getStopwatchMinutes(),
                                   ClockLogic::getStopwatchSeconds());
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void UITask(void *pvParameters)
    {
        (void)pvParameters;
        uint8_t cmd;

        for (;;)
        {
            // 1. Wait for a command for up to 100ms
            // This blocks the task, saving battery, but wakes up instantly on button press
            if (xQueueReceive(xUICommandQueue, &cmd, pdMS_TO_TICKS(100)) == pdPASS)
            {
                // Process input commands immediately
                if (cmd == UI_CMD_NEXT)
                {
                    incrementState();
                }
                else if (cmd == UI_CMD_PREV)
                {
                    decrementState();
                }
                else if (cmd == UI_CMD_STOPWATCH_TOGGLE)
                {
                    ClockLogic::toggleStopwatch();
                }
                else if (cmd == UI_CMD_STOPWATCH_RESET)
                {
                    ClockLogic::resetStopwatch();
                }
            }

            // 2. Render the current state
            // This runs after a command OR every 100ms (for clock/stopwatch updates)
            if (xSemaphoreTake(xDisplayMutex, portMAX_DELAY) == pdTRUE)
            {
                display.clearDisplay();

                // Centering slightly better: 144 width, text starts around 20
                drawViewAt(currentState, 20);

                display.refresh();
                xSemaphoreGive(xDisplayMutex);
            }
        }
    }

    void run()
    {
        Serial.println("UI Manager Starting...");
        xTaskCreate(UITask, "UITask", 2048, NULL, 1, NULL);
    }
}