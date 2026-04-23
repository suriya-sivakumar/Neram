#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <LSM6DS3.h>
#include "step_counter.h"
#include "system/logger.h"

static LSM6DS3 imu(I2C_MODE, 0x6A);

namespace StepCounter
{
    static volatile uint16_t currentSteps = 0;
    static volatile uint16_t dailyBaseline = 0;
    static volatile bool available = false;

    static uint16_t readRawStepCount()
    {
        uint8_t lo = 0, hi = 0;
        imu.readRegister(&lo, 0x4B);
        imu.readRegister(&hi, 0x4C);
        return ((uint16_t)hi << 8) | lo;
    }

    static void stepTask(void *pvParameters)
    {
        (void)pvParameters;
        uint16_t last = 0;
        for (;;)
        {
            uint16_t raw = readRawStepCount();
            uint16_t today = (raw >= dailyBaseline) ? (raw - dailyBaseline) : raw;
            currentSteps = today;

            if (today != last)
            {
                char buf[40];
                snprintf(buf, sizeof(buf), "steps: %u (+%d)",
                         (unsigned)today, (int)(today - last));
                LOG_I("STEP", buf);
                last = today;
            }

            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }

    void run()
    {
        // Enable IMU power rail on XIAO nRF52840 Sense (P1.08 via board pin 15)
        pinMode(PIN_LSM6DS3TR_C_POWER, OUTPUT);
        digitalWrite(PIN_LSM6DS3TR_C_POWER, HIGH);
        vTaskDelay(pdMS_TO_TICKS(50));

        if (imu.begin() != 0)
        {
            LOG_E("STEP", "IMU init failed");
            return;
        }

        // Pedometer register sequence per LSM6DS3TR-C datasheet
        uint8_t err = 0;
        // CTRL1_XL: 26 Hz ODR, ±2g FS, 200 Hz anti-aliasing BW
        err += imu.writeRegister(0x10, 0x20);
        // TAP_CFG (0x58): enable pedometer
        err += imu.writeRegister(0x58, 0x40);
        // CTRL10_C (0x19): FUNC_EN + PEDO_EN + reset pedometer
        err += imu.writeRegister(0x19, 0x14 | 0x02);
        vTaskDelay(pdMS_TO_TICKS(10));
        err += imu.writeRegister(0x19, 0x14);

        if (err != 0)
        {
            LOG_E("STEP", "pedometer cfg failed");
            return;
        }

        available = true;
        dailyBaseline = 0;
        currentSteps = 0;
        LOG_I("STEP", "pedometer ready");

        xTaskCreate(stepTask, "StepTask", 1024, NULL, 2, NULL);
    }

    uint16_t getSteps() { return currentSteps; }

    bool isAvailable() { return available; }

    void reset()
    {
        if (!available) return;
        // PEDO_RST_STEP pulse
        imu.writeRegister(0x19, 0x14 | 0x02);
        vTaskDelay(pdMS_TO_TICKS(5));
        imu.writeRegister(0x19, 0x14);
        dailyBaseline = 0;
        currentSteps = 0;
        LOG_I("STEP", "daily reset");
    }
}
