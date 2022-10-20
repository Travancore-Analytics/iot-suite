#pragma once
#ifndef Reachability_H
#define Reachability_H

#include "Arduino.h"
#include "ArduinoJson.h"
#include "IoTUtility.h"

#if defined(ARDUINO_ARCH_ESP32)
#include "BLEServer.h"
#include "WiFi.h"
#include "WebServer.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLE2902.h"
#include "EEPROM.h"
#elif defined(ARDUINO_SAMD_NANO_33_IOT)
#include "ArduinoBLE.h"
#include "WiFiNINA.h"
#include <vector>
#elif defined(ARDUINO_ARCH_ESP8266)
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "EEPROM.h"
#endif

#define ssid_index_start 0
#define ssid_index_stop 31
#define pswd_index_start 32
#define pswd_index_stop 95
#define response_code 200

#define HTTP_PORT 80
#define HTTPS_PORT 443
#define DEFAULT_INTERVAL 1000
#define ESP_EEPROM_SIZE 512

const String contentType("application/json");

enum networkStatus
{
    reachable,
    notReachable,
    skipReachable
};

enum connectionType
{
    ble,
    accessPoint
};
class ReachabilityBase
{
public:
    ReachabilityBase()
    {
        previousMilliSeconds = 0;
        sectoMilliSec = 0 ;
        isAdvertising = false;
        ssid="";
        password="";
    }
protected:
    unsigned long previousMilliSeconds;
    unsigned long currentMilliSeconds;
    unsigned long sectoMilliSec;
    bool isAdvertising;
    
    uint32_t interval;
    connectionType connType;
    String advertisingName;

    String ssid;
    String password;
};

#if defined(ARDUINO_SAMD_NANO_33_IOT)
class ReachabilityIoTArduinoNano33IoT : public ReachabilityBase
{
public:
    void initialize(connectionType type,uint32_t sec,String deviceName="");
    void loop();
   ~ReachabilityIoTArduinoNano33IoT();

private:
    void observe();
    bool isReachable();
    bool connectWifi();
    void handlePost();
    void readEEPROM();

    std::vector<String> split(String input, const char *delimiter);
    static void onWrite(BLEDevice central, BLECharacteristic characteristic);
    static void writeWifiEeprom(String ssid, String password);

    WiFiClient client;
    WiFiServer *server;
    StaticJsonDocument<250> jsonDocument;
};
extern class ReachabilityIoTArduinoNano33IoT reach;

#elif defined(ARDUINO_ARCH_ESP32)
class ReachabilityIoTESP32 : public ReachabilityBase
{
public:
    void initialize(connectionType type,uint32_t sec,String deviceName="");
    void loop();

protected:
    static void writeWifiEeprom(String ssid, String password);

private:
    void observe();
    bool isReachable();
    bool connectWifi();
    void handlePost();
    void readEEPROM();

    WebServer server;
    BLEServer *pServer;
    BLEService *iotService;
    BLECharacteristic *wifiData;
    BLEAdvertising *pAdvertising;
    StaticJsonDocument<250> jsonDocument;
};

class bleCallbacks : public ReachabilityIoTESP32, public BLECharacteristicCallbacks
{
private:
    void onWrite(BLECharacteristic *wifiData);
};
extern class ReachabilityIoTESP32 reach;

#elif defined(ARDUINO_ARCH_ESP8266)
class ReachabilityIoTESP8266 : public ReachabilityBase
{
public:
    void initialize(uint32_t sec,String deviceName);
    void loop();

protected:
    void writeWifiEeprom(String ssid, String password);

private:
    void observe();
    bool isReachable();
    bool connectWifi();
    void handlePost();
    void readEEPROM();

    ESP8266WebServer server;
    StaticJsonDocument<250> jsonDocument;
};
extern class ReachabilityIoTESP8266 reach;
#else
    #error "BOARD NOT SUPPORTED."
#endif //Class Declaration.

#endif //Reachability_H.
