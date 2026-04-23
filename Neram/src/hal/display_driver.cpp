#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include "board_config.h"
#include "hal/display_driver.h"
#include "system/clock_logic.h"

Adafruit_SharpMem display(SHARP_SCLK, SHARP_MOSI, SHARP_SS, 128, 128);

namespace DisplayDriver
{
    void initDisplay()
    {
        // Set pin modes for SPI interface
        pinMode(SHARP_SCLK, OUTPUT);
        pinMode(SHARP_MOSI, OUTPUT);
        pinMode(SHARP_SS, OUTPUT);

        // Set high drive mode for debugging as jumper cables decrease the pin voltage
        // NRF_P1->PIN_CNF[13] |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);
        // NRF_P1->PIN_CNF[11] |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);
        // NRF_P0->PIN_CNF[9] |= (GPIO_PIN_CNF_DRIVE_H0H1 << GPIO_PIN_CNF_DRIVE_Pos);

        display.begin();
        display.clearDisplay();
        display.refresh();
    }
}
