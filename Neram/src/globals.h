#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once
#include <FreeRTOS.h>
#include <semphr.h>

extern SemaphoreHandle_t xDisplayMutex;

extern QueueHandle_t xButtonQueue;
#define BTN_ID_A 1
#define BTN_ID_B 2
#define BTN_ID_C 3
#define BTN_ID_D 4

#endif