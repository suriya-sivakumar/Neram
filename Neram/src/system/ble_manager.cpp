#include <bluefruit.h>
#include "globals.h"
#include "ble_manager.h"
#include "system/clock_logic.h"

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
}

/* --- Callback Definitions --- */

void connect_callback(uint16_t conn_handle)
{
    BLEConnection *conn = Bluefruit.Connection(conn_handle);
    Serial.println("Connected");

    if (bleClientDis.discover(conn_handle))
    {
        Serial.println("DIS Discovered");
    }

    if (bleancs.discover(conn_handle))
    {
        Serial.println("ANCS Discovered, Requesting Pair...");
        conn->requestPairing();
    }

    if (bleCTime.discover(conn_handle))
    {
        Serial.println("CTS Discovered");
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
        Serial.println("Secured");
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
    char title[64] = {0};
    if (bleancs.getTitle(notif->uid, title, sizeof(title)))
    {
        Serial.printf("Notification: %s\n", title);
    }
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    Serial.printf("Disconnected, reason = 0x%02X\n", reason);
    bleClientDis.begin();
    bleancs.begin();
    bleCTime.begin();
}

void cts_adjust_callback(uint8_t reason)
{
    const char *reason_str[] = {"Manual", "External Reference", "Change of Time Zone", "Change of DST"};

    Serial.println("iOS Device time changed due to ");
    Serial.println(reason_str[reason]);
}
