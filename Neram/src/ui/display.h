#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include "hal/board_config.h"
#include "hal/display_driver.h"
#include "system/clock_logic.h"

namespace Display
{
    // Status bar reserves the top 12 px. All screen draws should render below y=12.
    static const int STATUS_BAR_HEIGHT = 12;

    void drawStatusBar(bool bleConnected, uint8_t batteryPct, uint16_t steps);
    void drawClock(int x, int y);
    void drawTimer(int x, int y);
    void drawStopwatch(int x, int y, uint8_t minutes, uint8_t seconds);
}
#endif // DISPLAY_H