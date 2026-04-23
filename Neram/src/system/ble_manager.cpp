#include <bluefruit.h>
#include "globals.h"
#include "ble_manager.h"
#include "system/clock_logic.h"
#include "system/logger.h"

void connect_callback(uint16_t conn_handle);
void disconnect_callback(uint16_t conn_handle, uint8_t reason);
void connection_secured_callback(uint16_t conn_handle);
void ancs_notification_callback(AncsNotification_t *notif);
void cts_adjust_callback(uint8_t reason);

// BLE Client Services
BLEClientDis bleClientDis;
BLEAncs bleancs;

// BLE Peripheral Services
BLEHidAdafruit blehid;
BLEUart bleuart;
BLEClientCts bleCTime;

namespace BLEManager
{
    void init();
    void startAdv();
    void sendMediaControl(uint16_t usage);

    void init()
    {
        Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
        Bluefruit.begin();
        Bluefruit.setTxPower(4);
        Bluefruit.setName("Neram");

        Bluefruit.Periph.setConnectCallback(connect_callback);
        Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
        Bluefruit.Security.setSecuredCallback(connection_secured_callback);

        bleClientDis.begin();
        bleCTime.begin();
        bleCTime.setAdjustCallback(cts_adjust_callback);
        bleancs.begin();
        bleancs.setNotificationCallback(ancs_notification_callback);
        blehid.begin();
        bleuart.begin();

        startAdv();
    }

    void startAdv()
    {
        Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
        Bluefruit.Advertising.addTxPower();
        Bluefruit.Advertising.addService(bleancs);
        Bluefruit.Advertising.addService(blehid);
        Bluefruit.Advertising.addService(bleuart);
        Bluefruit.ScanResponse.addName();
        Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_GENERIC_CLOCK);

        Bluefruit.Advertising.restartOnDisconnect(true);
        Bluefruit.Advertising.setInterval(32, 244);
        Bluefruit.Advertising.start(0);
    }

    void sendMediaControl(uint16_t usage)
    {
        if (Bluefruit.connected())
        {
            blehid.consumerKeyPress(usage);
            vTaskDelay(pdMS_TO_TICKS(50));
            blehid.consumerKeyRelease();
        }
    }

    bool isConnected()
    {
        return Bluefruit.connected() > 0;
    }
}

/* --- Callback Definitions --- */

void connect_callback(uint16_t conn_handle)
{
    BLEConnection *conn = Bluefruit.Connection(conn_handle);
    LOG_I("BLE", "Connected");

    if (bleClientDis.discover(conn_handle))
    {
        LOG_D("BLE", "DIS discovered");
    }

    if (bleancs.discover(conn_handle))
    {
        LOG_I("BLE", "ANCS discovered, requesting pair");
        conn->requestPairing();
    }

    if (bleCTime.discover(conn_handle))
    {
        LOG_I("BLE", "CTS discovered");
    }
}

void connection_secured_callback(uint16_t conn_handle)
{
    BLEConnection *conn = Bluefruit.Connection(conn_handle);
    if (!conn->secured())
    {
        conn->requestPairing();
    }
    else
    {
        LOG_I("BLE", "Link secured");
        if (bleancs.discovered())
        {
            bleancs.enableNotification();
        }
    }

    if (conn->secured() && bleCTime.discovered())
    {
        if (bleCTime.getCurrentTime())
        {
            ClockLogic::syncWithBLE(bleCTime);
        }
    }
}

void ancs_notification_callback(AncsNotification_t *notif)
{
    char title[60] = {0};
    if (bleancs.getTitle(notif->uid, title, sizeof(title)))
    {
        char buf[60];
        snprintf(buf, sizeof(buf), "notif: %.55s", title);
        LOG_I("BLE", buf);
    }
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "Disconnected reason=0x%02X", reason);
    LOG_W("BLE", buf);
    bleClientDis.begin();
    bleancs.begin();
    bleCTime.begin();
}

void cts_adjust_callback(uint8_t reason)
{
    const char *reason_str[] = {"Manual", "Ext Ref", "TZ change", "DST change"};
    char buf[40];
    snprintf(buf, sizeof(buf), "CTS adjust: %s", reason_str[reason & 3]);
    LOG_I("BLE", buf);
}
