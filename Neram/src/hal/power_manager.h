#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

namespace PowerManager
{
    void initPowerManagement();

    void initWatchdog();
    void feedWatchdog();
}

#endif // POWER_MANAGER_H