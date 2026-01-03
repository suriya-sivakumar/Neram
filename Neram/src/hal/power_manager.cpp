#include <Arduino.h>
#include "board_config.h"
#include <nrf_wdt.h>
#include <nrf_soc.h>
#include <hal/nrf_wdt.h>

namespace PowerManager
{
    void initPowerManagement()
    {
        // Initialize power management settings
        pinMode(VCC_ENABLE, OUTPUT);
        digitalWrite(VCC_ENABLE, HIGH);
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

        Serial.println("Watchdog initialized via CMSIS (5s timeout)");
    }

    void feedWatchdog()
    {
        // Feed the watchdog to prevent a reset
        NRF_WDT->RR[0] = WDT_RR_RR_Reload;
    }
}
