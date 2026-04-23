#include <Arduino.h>
#include <time.h>
#include <stdio.h>
#include <bluefruit.h>
#include "globals.h"
#include "clock_logic.h"
#include "hal/board_config.h"
#include "hal/input_manager.h"
#include "system/ble_manager.h"
#include "system/logger.h"

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

    void clockTask(void *pvParameters)
    {
        (void)pvParameters;
        // update the global time struct 't' once per second
        while (1)
        {
            if (stopwatch.running)
            {
                stopwatch.seconds++;
                if (stopwatch.seconds >= 60)
                {
                    stopwatch.seconds = 0;
                    stopwatch.minutes++;
                }
            }

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

            vTaskDelay(pdMS_TO_TICKS(1000));
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

        xTaskCreate(clockTask, "ClockTask", 4096, NULL, 4, NULL);
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
            LOG_I("CLK", "Stopwatch reset");
        }
    }

    void toggleStopwatch()
    {
        if (!stopwatch.running)
        {
            stopwatch.running = true;
            LOG_I("CLK", "Stopwatch started");
        }
        else
        {
            stopwatch.running = false;
            char buf[40];
            snprintf(buf, sizeof(buf), "Stopwatch stopped %02d:%02d",
                     stopwatch.minutes, stopwatch.seconds);
            LOG_I("CLK", buf);
        }
    }

    void syncWithBLE(BLEClientCts &bleCTime)
    {
        LOG_I("CLK", "Syncing time via BLE CTS...");

        t.tm_sec = bleCTime.Time.second;
        t.tm_min = bleCTime.Time.minute;
        t.tm_hour = bleCTime.Time.hour;
        t.tm_mday = bleCTime.Time.day;
        t.tm_mon = bleCTime.Time.month - 1;
        t.tm_year = bleCTime.Time.year - 1900;

        mktime((struct tm *)&t);

        char buf[40];
        snprintf(buf, sizeof(buf), "Time synced %02d:%02d:%02d",
                 t.tm_hour, t.tm_min, t.tm_sec);
        LOG_I("CLK", buf);
    }

}