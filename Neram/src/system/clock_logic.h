#ifndef CLOCK_LOGIC_H
#define CLOCK_LOGIC_H

#include <Arduino.h>
#include <time.h>
#include "hal/board_config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

namespace ClockLogic
{
    extern volatile struct tm t;

    void timerCallback(TimerHandle_t xTimer);

    void initClock();

}

#endif // CLOCK_LOGIC_H