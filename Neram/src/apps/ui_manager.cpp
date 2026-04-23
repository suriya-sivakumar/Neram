#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>

#include "apps/ui_manager.h"
#include "ui/display.h"
#include "globals.h"
#include "system/clock_logic.h"
#include "system/logger.h"
#include "system/ble_manager.h"
#include "system/step_counter.h"
#include "hal/power_manager.h"

namespace UIManager
{
    WATCH_STATE currentState = WATCH_STATE::CLOCK;

    static const char *stateName(WATCH_STATE s) {
        switch (s) {
            case CLOCK:     return "CLOCK";
            case TIMER:     return "TIMER";
            case STOPWATCH: return "STOPWATCH";
            default:        return "UNKNOWN";
        }
    }

    void setState(WATCH_STATE state) {
        if (state != currentState) {
            char buf[48];
            snprintf(buf, sizeof(buf), "%s -> %s", stateName(currentState), stateName(state));
            LOG_I("UI", buf);
        }
        currentState = state;
    }

    WATCH_STATE getState() { return currentState; }

    void incrementState()
    {
        WATCH_STATE prev = currentState;
        int next = static_cast<int>(currentState) + 1;
        if (next >= static_cast<int>(WATCH_STATE::COUNT))
        {
            currentState = WATCH_STATE::CLOCK;
        }
        else
        {
            currentState = static_cast<WATCH_STATE>(next);
        }
        char buf[48];
        snprintf(buf, sizeof(buf), "%s -> %s (next)", stateName(prev), stateName(currentState));
        LOG_I("UI", buf);
    }

    void decrementState()
    {
        WATCH_STATE prev = currentState;
        int next = static_cast<int>(currentState) - 1;
        if (next < 0)
        {
            currentState = static_cast<WATCH_STATE>(static_cast<int>(WATCH_STATE::COUNT) - 1);
        }
        else
        {
            currentState = static_cast<WATCH_STATE>(next);
        }
        char buf[48];
        snprintf(buf, sizeof(buf), "%s -> %s (prev)", stateName(prev), stateName(currentState));
        LOG_I("UI", buf);
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
            if (xQueueReceive(xUICommandQueue, &cmd, pdMS_TO_TICKS(100)) == pdPASS)
            {
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
                    LOG_I("UI", "cmd: STOPWATCH_TOGGLE");
                    ClockLogic::toggleStopwatch();
                }
                else if (cmd == UI_CMD_STOPWATCH_RESET)
                {
                    LOG_I("UI", "cmd: STOPWATCH_RESET");
                    ClockLogic::resetStopwatch();
                }
            }

            if (xSemaphoreTake(xDisplayMutex, portMAX_DELAY) == pdTRUE)
            {
                display.clearDisplay();
                drawViewAt(currentState, 20);
                Display::drawStatusBar(BLEManager::isConnected(),
                                       PowerManager::getBatteryPercent(),
                                       StepCounter::getSteps());
                display.refresh();
                xSemaphoreGive(xDisplayMutex);
            }
        }
    }

    void run()
    {
        LOG_I("UI", "UIManager starting");
        xTaskCreate(UITask, "UITask", 4096, NULL, 1, NULL);
    }
}