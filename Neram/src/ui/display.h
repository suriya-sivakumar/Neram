#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include "hal/board_config.h"
#include "hal/display_driver.h"
#include "system/clock_logic.h"

namespace Display
{
    void drawClock(void);
    void notifyButtonPress(void);
    void drawTimer(void);
    void drawStopwatch(void);
}
#endif // DISPLAY_H