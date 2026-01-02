#include <Arduino.h>
#include "ui/display.h"
#include "hal/board_config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

namespace Display
{
    void drawClock(int x, int y)
    {
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d:%02d", ClockLogic::t.tm_hour, ClockLogic::t.tm_min, ClockLogic::t.tm_sec);

        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(x, y);
        display.print(timeStr);

        display.refresh();
        delay(500); // Update every 500 ms
    }

    void drawTimer(int x, int y)
    {
        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(x, y);
        display.print("Timer: ");
        // display.print(ClockLogic::getTimerValue());
        display.refresh();
    }

    void drawStopwatch(int x, int y, uint8_t minutes, uint8_t seconds)
    {
        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(x, y);
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d", minutes, seconds);
        display.print(timeStr);
        display.refresh();
    }

}