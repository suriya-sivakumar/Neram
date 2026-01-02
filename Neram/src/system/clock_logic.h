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
    extern volatile struct stopwatch_t stopwatch;

    void timerCallback(TimerHandle_t xTimer);

    void initClock();

    uint8_t getStopwatchMinutes();
    uint8_t getStopwatchSeconds();

    void resetStopwatch();
    void toggleStopwatch();

}

#endif // CLOCK_LOGIC_H