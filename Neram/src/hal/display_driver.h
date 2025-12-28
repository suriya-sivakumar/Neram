#ifndef DISPLAY_DRIVER_H
#define DISPLAY_DRIVER_H

#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include "board_config.h"
#include "system/clock_logic.h"

extern Adafruit_SharpMem display;

namespace DisplayDriver
{
    void initDisplay();
}

#endif // DISPLAY_DRIVER_H