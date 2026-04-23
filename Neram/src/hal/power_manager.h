#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

namespace PowerManager
{
    static const uint8_t BATTERY_ABSENT = 0xFF;

    void initPowerManagement();

    void initWatchdog();
    void feedWatchdog();

    void initBattery();
    uint8_t getBatteryPercent();
}

#endif // POWER_MANAGER_H