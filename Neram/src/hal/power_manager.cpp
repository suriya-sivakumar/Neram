#include <Arduino.h>
#include "board_config.h"

namespace PowerManager
{
    void initPowerManagement()
    {
        // Initialize power management settings
        pinMode(VCC_ENABLE, OUTPUT);
        digitalWrite(VCC_ENABLE, HIGH);
        delay(100);
    }
}
