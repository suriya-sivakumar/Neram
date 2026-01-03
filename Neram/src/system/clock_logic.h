#ifndef CLOCK_LOGIC_H
#define CLOCK_LOGIC_H

#include <Arduino.h>
#include <time.h>
#include <bluefruit.h>
#include "hal/board_config.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

class BLEClientCts;
namespace ClockLogic
{
    extern volatile struct tm t;
    extern volatile struct stopwatch_t stopwatch;

    void clockTask(void *pvParameters);

    void initClock();

    uint8_t getStopwatchMinutes();
    uint8_t getStopwatchSeconds();

    void resetStopwatch();
    void toggleStopwatch();

    void syncWithBLE(BLEClientCts &bleCTime);

}

#endif // CLOCK_LOGIC_H