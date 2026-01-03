#include <Arduino.h>
#include <time.h>
#include <bluefruit.h>
#include "globals.h"
#include "clock_logic.h"
#include "hal/board_config.h"
#include "hal/input_manager.h"
#include "system/ble_manager.h"

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

    void syncWithBLE(BLEClientCts &bleCTime)
    {
        Serial.println("Syncing clock with Phone via BLE...");

        // Pause the timer or use a mutex if you have many tasks
        // to prevent reading 't' while it's being updated.

        t.tm_sec = bleCTime.Time.second;
        t.tm_min = bleCTime.Time.minute;
        t.tm_hour = bleCTime.Time.hour;
        t.tm_mday = bleCTime.Time.day;
        t.tm_mon = bleCTime.Time.month - 1;    // tm_mon is 0-11
        t.tm_year = bleCTime.Time.year - 1900; // tm_year is years since 1900

        // Use mktime to "normalize" the struct (calculates day of week/year)
        mktime((struct tm *)&t);

        Serial.println("Clock synced with Phone via BLE!");
    }

}