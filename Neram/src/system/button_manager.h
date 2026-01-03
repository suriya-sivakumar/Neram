#ifndef BUTTON_MANAGER_H
#define BUTTON_MANAGER_H

#include <Arduino.h>
#include <FreeRTOS.h>

namespace ButtonManager
{
    void run();
    void buttonTask(void *pvParameters);
    void attachButtonISR();
}

#endif // BUTTON_MANAGER_H