#include "apps/clock.h"
#include "ui/display.h"

namespace Clock
{
    void clockTask(void *pvParameters)
    {
        (void)pvParameters;
        for (;;)
        {
            Display::drawClock();
            Serial.println("ClockTask: Tick");
            // TODO: Change to interrupt based pre-emption
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
    void run()
    {
        xTaskCreate(clockTask, "ClockTask", 2048, NULL, 1, NULL);
    }
}
