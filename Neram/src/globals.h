#ifndef GLOBALS_H
#define GLOBALS_H

#pragma once
#include <FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

extern SemaphoreHandle_t xDisplayMutex;
extern QueueHandle_t xButtonQueue;
extern QueueHandle_t xUICommandQueue;

// Command definitions
#define UI_CMD_NEXT 1
#define UI_CMD_PREV 2
#define UI_CMD_STOPWATCH_TOGGLE 3 // Use one button to Start/Stop
#define UI_CMD_STOPWATCH_RESET 4

#define BTN_ID_A 1
#define BTN_ID_B 2
#define BTN_ID_C 4
#define BTN_ID_D 8

#endif