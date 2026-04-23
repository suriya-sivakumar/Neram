#ifndef SYSTEM_LOGGER_H
#define SYSTEM_LOGGER_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <queue.h>

// Log levels
#define LOG_DEBUG 0
#define LOG_INFO  1
#define LOG_WARN  2
#define LOG_ERROR 3

// Convenience macros — safe to call from any task
#define LOG_D(tag, msg) Logger::log(LOG_DEBUG, tag, msg)
#define LOG_I(tag, msg) Logger::log(LOG_INFO,  tag, msg)
#define LOG_W(tag, msg) Logger::log(LOG_WARN,  tag, msg)
#define LOG_E(tag, msg) Logger::log(LOG_ERROR, tag, msg)

typedef struct {
    uint32_t tick;   // xTaskGetTickCount() at time of log call
    uint8_t  level;
    char     tag[12];
    char     msg[60];
} LogMessage_t;

extern QueueHandle_t xLogQueue;

namespace Logger {
    void init();
    // Safe to call from any task (not ISR). Drops message if queue full.
    void log(uint8_t level, const char *tag, const char *msg);
}

#endif // SYSTEM_LOGGER_H
