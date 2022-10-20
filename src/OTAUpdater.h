#ifndef OTAUpdater_H
#define OTAUpdater_H

#include "Arduino.h"
#include "ArduinoJson.h"
#include <vector>

#if defined(ARDUINO_ARCH_SAMD)
#include "ArduinoOTA.h"
#include "WiFiNINA.h"
#include "ArduinoHttpClient.h"
#include "IoTUtility.h"

#elif defined(ARDUINO_ARCH_ESP32)
#include "Update.h"
#include "HTTPClient.h"
#include "EEPROM.h"

#elif defined(ARDUINO_ARCH_ESP8266)
#include "ESP8266httpUpdate.h"
#include "EEPROM.h"
#include "ESP8266HTTPClient.h"
#endif

#define index_start 96
#define index_stop 100
#define http_resp_ok 200
#define json_delimiter "/"
#define json_max_length 384

#define HTTP_PORT  80 
#define HTTPS_PORT 443
#define HTTP_RESPONSE_SUCCESS 200
#define DEFAULT_INTERVAL 1000
#define ESP_EEPROM_SIZE 512

enum updateStatus
{
  noUpdate,
  newUpdate,
  skip
};

class OTAUpdaterBase
{
protected:
  String jsonUrlPath;
  String jsonUrl;
  String jsonVersionPath;
  String jsonVersionNumber;
  StaticJsonDocument<384> doc;
  JsonObject root;
  const char *ckey;
  std::vector<String> tokenData;
  unsigned long previousMilliSeconds = 0;
  unsigned long currentMilliSeconds;

  std::vector<String> split(String input, const char *delimiter);
  String parseJson(StaticJsonDocument<384> doc, String data);
  JsonVariant findNestedKey(JsonObject obj, const char *key);
  signed int versionCompare(String eversionNumber, String jsonVersionNumber);
};

/**
 * @brief OTAUpdater class for esp32
 *
 */
class OTAUpdaterIoTESP32 : protected OTAUpdaterBase
{
#if defined(ARDUINO_ARCH_ESP32)
  int totalLength;       // total size of firmware
  int currentLength = 0; // current size of written firmware
  StaticJsonDocument<json_max_length> readJson(String path);
  String readEepromVersion();
  void rewriteEepromVersionNumber(String jsonVersionNumber);
  void updateFirmware(uint8_t *data, size_t len);

public:
  virtual void initialize(String serverUrl, String serverVersionNumber, String path);
  virtual updateStatus checkForUpdate(unsigned long interval);
  virtual void updateCode();
#endif
};

/**
 * @brief  OTAUpdater class for esp8266
 *
 */
class OTAUpdaterIoTESP8266 : protected OTAUpdaterBase
{
#if defined(ARDUINO_ARCH_ESP8266)
  String readEepromVersion();
  StaticJsonDocument<json_max_length> readJson(String path);
  void rewriteEepromVersionNumber(String jsonVersionNumber);
  void updateFinished();

public:
  virtual void initialize(String serverUrl, String serverVersionNumber, String path);
  virtual updateStatus checkForUpdate(unsigned long interval);
  virtual void updateCode();
#endif
};

/**
 * @brief  OTAUpdater class for arduino nano 33 iot
 *
 */
class OTAUpdaterIoTArduinoNano33IoT : protected OTAUpdaterBase
{
#if defined(ARDUINO_ARCH_SAMD)
  StaticJsonDocument<json_max_length> readJson(String path);

public:
  virtual void initialize(String serverUrl, String serverVersionNumber, String path);
  virtual updateStatus checkForUpdate(unsigned long interval , String eepromVersionNumber);
  virtual void updateCode();
#endif
};

class OTAUpdater : public OTAUpdaterIoTESP32, public OTAUpdaterIoTESP8266, public OTAUpdaterIoTArduinoNano33IoT
{
};
#endif
