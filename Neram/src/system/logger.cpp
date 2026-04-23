#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
#include <string.h>

#include "system/logger.h"

QueueHandle_t xLogQueue = NULL;

static const char *levelStr(uint8_t level) {
    switch (level) {
        case LOG_DEBUG: return "D";
        case LOG_INFO:  return "I";
        case LOG_WARN:  return "W";
        case LOG_ERROR: return "E";
        default:        return "?";
    }
}

namespace Logger {

    static void vLogTask(void *pvParameters) {
        (void)pvParameters;
        LogMessage_t msg;
        for (;;) {
            if (xQueueReceive(xLogQueue, &msg, portMAX_DELAY) == pdPASS) {
                // Format: [tick_ms][L][TAG] message
                Serial.print('[');
                Serial.print(msg.tick);
                Serial.print("][");
                Serial.print(levelStr(msg.level));
                Serial.print("][");
                Serial.print(msg.tag);
                Serial.print("] ");
                Serial.println(msg.msg);
            }
        }
    }

    void init() {
        xLogQueue = xQueueCreate(32, sizeof(LogMessage_t));
        if (xLogQueue == NULL) {
            Serial.println("FATAL: log queue alloc failed");
            return;
        }
        // Priority 1 — lower than clock (4) and button (2), never blocks producers
        xTaskCreate(vLogTask, "LogTask", 1024, NULL, 1, NULL);
    }

    void log(uint8_t level, const char *tag, const char *msg) {
        if (xLogQueue == NULL) return;

        LogMessage_t entry;
        entry.tick  = (uint32_t)xTaskGetTickCount();
        entry.level = level;
        strncpy(entry.tag, tag,  sizeof(entry.tag)  - 1);
        strncpy(entry.msg, msg,  sizeof(entry.msg)  - 1);
        entry.tag[sizeof(entry.tag) - 1] = '\0';
        entry.msg[sizeof(entry.msg) - 1] = '\0';

        // Non-blocking: drop if queue full (logging must never stall a task)
        xQueueSend(xLogQueue, &entry, 0);
    }
}
