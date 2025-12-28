#include <Arduino.h>
#include "clock_logic.h"
#include "hal/board_config.h"

SoftwareTimer timer;

namespace ClockLogic
{

    volatile struct tm t;
    void timerCallback(TimerHandle_t xTimer)
    {
        (void)xTimer;
        // update the global time struct 't' once per second

        t.tm_sec++;
        if (t.tm_sec >= 60)
        {
            t.tm_sec = 0;
            t.tm_min++;
            if (t.tm_min >= 60)
            {
                t.tm_min = 0;
                t.tm_hour++;
                if (t.tm_hour >= 24)
                {
                    t.tm_hour = 0;
                    t.tm_mday++;
                    if (t.tm_mday > 31)
                    {
                        t.tm_mday = 1;
                        t.tm_mon++;
                        if (t.tm_mon > 11)
                        {
                            t.tm_mon = 0;
                            t.tm_year++;
                        }
                    }
                }
            }
        }
    }

    void initClock()
    {
        t.tm_year = 2025 - 1900;
        t.tm_mon = 11;
        t.tm_mday = 28;
        t.tm_hour = 9;
        t.tm_min = 42;
        t.tm_sec = 0;

        timer.begin(1000, timerCallback, NULL, true);

        timer.start();
    }
}