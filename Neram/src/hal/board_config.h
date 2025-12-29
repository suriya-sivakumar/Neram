#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#pragma once
#include "Arduino.h"

// This project uses a nice!nano clone board which uses a nRF52840

// VCC enable pin (to power the VCC rail)
#define VCC_ENABLE PIN_013

// BW color definitions
#define BLACK 0
#define WHITE 1

// SPI pinmap
#define SHARP_SCLK PIN_113
#define SHARP_MOSI PIN_111
#define SHARP_SS PIN_009

// Button pinmap

#define BUTTON_A PIN_024
#define BUTTON_B PIN_022
#define BUTTON_C PIN_020
#define BUTTON_D PIN_017

#endif