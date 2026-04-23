#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H
#include "Arduino.h"

// GPIO definitions for the watch, for both the nice!nano and the XIAO NRF52840 Sense.

/*!SECTION XIAO NRF52840 Sense */

// // VCC enable pin (to power the VCC rail)
#define VCC_ENABLE 1

// BW color definitions
#define BLACK 0
#define WHITE 1

// // SPI pinmap
#define SHARP_SCLK D8
#define SHARP_MOSI D10
#define SHARP_SS D9

// Button pinmap

#define BUTTON_A D0
#define BUTTON_B D1
#define BUTTON_C D2
#define BUTTON_D D3

// Battery sense (XIAO NRF52840 Sense: P0.31 via 1M:510k divider gated by P0.14)
#define PIN_VBAT_SENSE  32  // P0.31 analog input
#define PIN_VBAT_ENABLE 14  // P0.14 - drive LOW to enable divider
#define VBAT_DIVIDER    (1510.0f / 510.0f)  // (R1+R2)/R2
#define VBAT_MV_EMPTY   3300
#define VBAT_MV_FULL    4200

/*!SECTION NICE!NANO */

// // VCC enable pin (to power the VCC rail)
// #define VCC_ENABLE PIN_013

// // BW color definitions
// #define BLACK 0
// #define WHITE 1

// // SPI pinmap
// #define SHARP_SCLK PIN_113
// #define SHARP_MOSI PIN_111
// #define SHARP_SS PIN_009

// // Button pinmap

// #define BUTTON_A PIN_024
// #define BUTTON_B PIN_022
// #define BUTTON_C PIN_020
// #define BUTTON_D PIN_017

// SPI pinmap

#endif