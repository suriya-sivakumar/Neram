#include <bluefruit.h>
#include "clock_logic.h"

BLEUart bleuart; // Create the UART service object

// TODO: Make this not UART

namespace BLEManager
{

    void init()
    {
        Bluefruit.begin();
        Bluefruit.setTxPower(4); // 4dBm is a good balance for watch range
        Bluefruit.setName("Neram");

        // Start the UART service
        bleuart.begin();

        // Setup Advertising
        Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
        Bluefruit.Advertising.addService(bleuart);
        Bluefruit.ScanResponse.addName();

        Bluefruit.Advertising.start(0);
    }

    void parseUartTime()
    {
        if (bleuart.available() >= 11)
        {
            char header = bleuart.read();
            if (header == 'T')
            {
                uint32_t timestamp = 0;
                char buffer[11];
                bleuart.read(buffer, 10);
                buffer[10] = '\0';

                timestamp = strtoul(buffer, NULL, 10);

                time_t rawtime = (time_t)timestamp;
                struct tm *timeinfo = localtime(&rawtime);

                // Update your global clock
                ClockLogic::t.tm_sec = timeinfo->tm_sec;
                ClockLogic::t.tm_min = timeinfo->tm_min;
                ClockLogic::t.tm_hour = (timeinfo->tm_hour + 11) % 24;
                ClockLogic::t.tm_mday = timeinfo->tm_mday;
                ClockLogic::t.tm_mon = timeinfo->tm_mon;
                ClockLogic::t.tm_year = timeinfo->tm_year;
                ClockLogic::t.tm_wday = timeinfo->tm_wday;
                ClockLogic::t.tm_yday = timeinfo->tm_yday;
                ClockLogic::t.tm_isdst = timeinfo->tm_isdst;
                Serial.println("Time Synced via UART!");
            }
        }
    }
}