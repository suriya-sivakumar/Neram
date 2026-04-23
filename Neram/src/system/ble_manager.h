#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

namespace BLEManager
{
    void init();
    void startAdv();
    void sendMediaControl(uint16_t usage);
    bool isConnected();
}

#endif // BLE_MANAGER_H