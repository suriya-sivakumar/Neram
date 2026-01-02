#ifndef INPUT_MANAGER_H_
#define INPUT_MANAGER_H_

#include <Arduino.h>

namespace InputManager
{
    void init();
    extern void buttonISR();
}

#endif // INPUT_MANAGER_H_
