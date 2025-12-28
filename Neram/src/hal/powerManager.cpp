#include <Arduino.h>
#include "boardConfig.h"

void initPowerManagement()
{
    // Initialize power management settings
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(100);
}
