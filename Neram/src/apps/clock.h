#ifndef SRC_APPS_CLOCK_H_
#define SRC_APPS_CLOCK_H_

#include <time.h>
#include <Arduino.h>

namespace Clock
{
    void run();

    void clockTask(void *pvParameters);
}

#endif // SRC_APPS_CLOCK_H_