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

        if (stopwatch.running)
        {
            stopwatch.seconds++;
            if (stopwatch.seconds >= 60)
            {
                stopwatch.seconds = 0;
                stopwatch.minutes++;
            }
        }

        struct tm local_t;
        local_t.tm_sec = t.tm_sec;
        local_t.tm_min = t.tm_min;
        local_t.tm_hour = t.tm_hour;
        local_t.tm_mday = t.tm_mday;
        local_t.tm_mon = t.tm_mon;
        local_t.tm_year = t.tm_year;

        time_t now = mktime(&local_t);
        now++;
        struct tm *next = localtime(&now);

        t.tm_sec = next->tm_sec;
        t.tm_min = next->tm_min;
        t.tm_hour = next->tm_hour;
        t.tm_mday = next->tm_mday;
        t.tm_mon = next->tm_mon;
        t.tm_year = next->tm_year;
        t.tm_wday = next->tm_wday;
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