#ifndef SYSTEM_STEP_COUNTER_H
#define SYSTEM_STEP_COUNTER_H

#include <stdint.h>

namespace StepCounter
{
    void run();                 // Start IMU + polling task
    uint16_t getSteps();        // Daily step count (0 if IMU unavailable)
    void reset();               // Clear hardware counter + daily baseline
    bool isAvailable();
}

#endif // SYSTEM_STEP_COUNTER_H
