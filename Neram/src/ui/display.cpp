#include <Arduino.h>
#include "ui/display.h"
#include "hal/board_config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

namespace Display
{
    void drawClock(void)
    {
        char timeStr[10];
        sprintf(timeStr, "%02d:%02d:%02d", ClockLogic::t.tm_hour, ClockLogic::t.tm_min, ClockLogic::t.tm_sec);

        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(20, 50);
        display.print(timeStr);

        display.refresh();
        delay(500); // Update every 500 ms
    }

    void drawTimer(void)
    {
        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(20, 50);
        display.print("Timer: ");
        // display.print(ClockLogic::getTimerValue());
        display.refresh();
    }

    void drawStopwatch(void)
    {
        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(20, 50);
        display.print("Stopwatch: ");
        // display.print(ClockLogic::getStopwatchValue());
        display.refresh();
    }

    void notifyButtonPress(void)
    {
        display.clearDisplay();
        display.setTextColor(0); // BLACK
        display.setTextSize(2);
        display.setCursor(20, 50);
        display.print("Button Pressed!");

        display.refresh();
    }
}