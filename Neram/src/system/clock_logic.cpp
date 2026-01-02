#include <Arduino.h>
#include "clock_logic.h"
#include "hal/board_config.h"

SoftwareTimer timer;

namespace ClockLogic
{

    volatile struct tm t;
    struct stopwatch_t
    {
        int minutes;
        int seconds;
        bool running;
    };
    volatile stopwatch_t stopwatch = {0, 0, false};

    void timerCallback(TimerHandle_t xTimer)
    {
        (void)xTimer;
        // update the global time struct 't' once per second

        t.tm_sec++;

        if (stopwatch.running)
        {
            stopwatch.seconds++;
            if (stopwatch.seconds >= 60)
            {
                stopwatch.seconds = 0;
                stopwatch.minutes++;
            }
        }

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

    uint8_t getStopwatchMinutes()
    {
        return stopwatch.minutes;
    }

    uint8_t getStopwatchSeconds()
    {
        return stopwatch.seconds;
    }

    void resetStopwatch()
    {
        if (!stopwatch.running)
        {
            stopwatch.minutes = 0;
            stopwatch.seconds = 0;
            stopwatch.running = false;
        }
    }

    void toggleStopwatch()
    {
        if (!stopwatch.running)
        {
            stopwatch.running = true;
        }
        else
        {
            stopwatch.running = false;
        }
    }

}