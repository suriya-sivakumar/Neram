#ifndef SRC_APPS_UI_MANAGER_H_
#define SRC_APPS_UI_MANAGER_H_

#include <time.h>
#include <Arduino.h>

namespace UIManager
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

    void drawViewAt(WATCH_STATE state, int x);

    void performTransition(WATCH_STATE from, WATCH_STATE to, int offset);

}

#endif // SRC_APPS_UI_MANAGER_H_