#ifndef SRC_APPS_UI_MANAGER_H_
#define SRC_APPS_UI_MANAGER_H_

#include <time.h>
#include <Arduino.h>

namespace UI_Manager
{
    extern enum WATCH_STATE {
        CLOCK,
        TIMER,
        STOPWATCH,
        COUNT // to be used for incrementing the state
    } currentState;

    void setState(WATCH_STATE state);
    WATCH_STATE getState();

    void incrementState();
    void decrementState();

    void run();

    void UITask(void *pvParameters);
}

#endif // SRC_APPS_UI_MANAGER_H_