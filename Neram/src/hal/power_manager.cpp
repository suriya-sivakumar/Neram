#include <Arduino.h>
#include "board_config.h"
#include "power_manager.h"
#include <nrf_wdt.h>
#include <nrf_soc.h>
#include <hal/nrf_wdt.h>
#include "system/logger.h"

namespace PowerManager
{
    void initPowerManagement()
    {
        // Initialize power management settings
        // pinMode(VCC_ENABLE, OUTPUT);
        // digitalWrite(VCC_ENABLE, HIGH);
    }

    void initWatchdog()
    {
        // Configure WDT to run even when CPU is sleeping or halted by debugger
        NRF_WDT->CONFIG = (WDT_CONFIG_SLEEP_Run << WDT_CONFIG_SLEEP_Pos) |
                          (WDT_CONFIG_HALT_Run << WDT_CONFIG_HALT_Pos);

        // Set timeout (32768 ticks = 1 second). 5 seconds total.
        NRF_WDT->CRV = 5 * 32768;

        // Enable reload request for channel 0
        NRF_WDT->RREN = WDT_RREN_RR0_Enabled << WDT_RREN_RR0_Pos;

        // Start the Watchdog
        NRF_WDT->TASKS_START = 1;

        LOG_I("PWR", "WDT init (5s timeout)");
    }

    void feedWatchdog()
    {
        // Feed the watchdog to prevent a reset
        NRF_WDT->RR[0] = WDT_RR_RR_Reload;
    }

    void initBattery()
    {
        pinMode(PIN_VBAT_ENABLE, OUTPUT);
        digitalWrite(PIN_VBAT_ENABLE, LOW);
        analogReference(AR_INTERNAL_3_0);
        analogReadResolution(12);
        LOG_I("PWR", "Battery sense init");
    }

    uint8_t getBatteryPercent()
    {
        // Detect charge-IC pulsing (no battery present) by tracking sample spread
        // across several UI frames. A real battery is stable to within ~50 mV.
        static const int N = 8;
        static int samples[N] = {0};
        static int idx = 0;
        static int count = 0;

        int raw = analogRead(PIN_VBAT_SENSE);
        int mv = (int)((raw * 3000.0f / 4095.0f) * VBAT_DIVIDER);
        samples[idx] = mv;
        idx = (idx + 1) % N;
        if (count < N) count++;

        if (count < N) return BATTERY_ABSENT;

        int min_mv = samples[0], max_mv = samples[0];
        long sum = 0;
        for (int i = 0; i < N; i++) {
            if (samples[i] < min_mv) min_mv = samples[i];
            if (samples[i] > max_mv) max_mv = samples[i];
            sum += samples[i];
        }
        int avg = sum / N;

        if ((max_mv - min_mv) > 200 || avg < 3000 || avg > 4300) return BATTERY_ABSENT;

        int pct = ((avg - VBAT_MV_EMPTY) * 100) / (VBAT_MV_FULL - VBAT_MV_EMPTY);
        if (pct < 0) pct = 0;
        if (pct > 100) pct = 100;
        return (uint8_t)pct;
    }
}
