# Project Neram: nRF52840 BLE Wearable (FreeRTOS)

## Build & Test Commands
- **Build Firmware:** `pio run`
- **Flash Hardware:** `pio run --target upload`
- **Monitor Serial:** `pio device monitor`
- **Clean Project:** `pio run --target clean`
- **Debug:** `pio debug`
- **Linter:** `pio check`

## Tech Stack & Architecture
- **Framework:** FreeRTOS (Vanilla or via Nordic SDK)
- **Build System:** PlatformIO (PIO)
- **Hardware:** nRF52840 (Cortex-M4F), Sharp Memory LCD (SPI)
- **BLE Stack:** Nordic SoftDevice (e.g., S140) or NimBLE.
- **Task Management:** Check `src/main.c` for `xTaskCreate` definitions and priority levels.

## Code Style & Patterns
- **Standard:** C99 / C11.
- **FreeRTOS API:** Use `vTaskDelay`, `xQueueSend`, `xSemaphoreTake`, etc.
- **Naming:** `vFunctionName` for void, `pxPointerName` for pointers (FreeRTOS style).
- **Memory:** Prefer `pvPortMalloc` / `vPortFree` if dynamic allocation is required, but stick to static tasks where possible.
- **Error Handling:** Check `pdPASS` or `errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`.

## Hardware Mapping (Pinout)
- **LCD SPI:** SCK (P0.XX), MOSI (P0.XX), CS (P0.XX).
- **VCOM Toggle:** Handled in a dedicated high-priority FreeRTOS task or timer callback.
- **Buttons:** P0.11, P0.12 (Configured via GPIOTE interrupts).
- **Battery:** SAADC on P0.02.

## Important Context
- Configuration settings are in `platformio.ini` and `include/FreeRTOSConfig.h`.
- The SoftDevice (if used) requires specific memory offsets defined in the linker script.
- Do not modify files in `.pio/` or `.vscode/`.