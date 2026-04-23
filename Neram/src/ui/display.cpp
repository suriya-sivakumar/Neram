#include <Arduino.h>
#include "ui/display.h"
#include "hal/board_config.h"
#include "hal/power_manager.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

namespace Display
{
    void drawStatusBar(bool bleConnected, uint8_t batteryPct, uint16_t steps)
    {
        display.setTextColor(0); // BLACK
        display.setTextSize(1);

        display.setCursor(2, 2);
        display.print(bleConnected ? "BLE" : "---");

        char pctStr[8];
        if (batteryPct == PowerManager::BATTERY_ABSENT) {
            strcpy(pctStr, "USB");
        } else {
            snprintf(pctStr, sizeof(pctStr), "%u%%", (unsigned)batteryPct);
        }
        int pctW = (int)strlen(pctStr) * 6;
        display.setCursor(128 - pctW - 2, 2);
        display.print(pctStr);

        char stepStr[8];
        snprintf(stepStr, sizeof(stepStr), "%u", (unsigned)steps);
        int stepW = (int)strlen(stepStr) * 6;
        display.setCursor((128 - stepW) / 2, 2);
        display.print(stepStr);

        display.drawLine(0, STATUS_BAR_HEIGHT - 1, 128, STATUS_BAR_HEIGHT - 1, 0);
    }

    void drawClock(int x, int y)
    {
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d:%02d", ClockLogic::t.tm_hour, ClockLogic::t.tm_min, ClockLogic::t.tm_sec);

        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(x, y);
        display.print(timeStr);
    }

    void drawTimer(int x, int y)
    {
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(x, y);
        display.print("Timer: ");
    }

    void drawStopwatch(int x, int y, uint8_t minutes, uint8_t seconds)
    {
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(x, y);
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d", minutes, seconds);
        display.print(timeStr);
    }

}