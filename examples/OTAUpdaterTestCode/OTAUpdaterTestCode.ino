#include "IoTSuite.h"

#define currentVersionNumber "1.1.1"
#define SSID "your_ssid"
#define PASSWORD "Network_password"

OTAUpdater ota; //Creating OTA session object.
String url = "https://d1lbp6ltb9vqea.cloudfront.net/IoTSuite.json";

unsigned long ledpreviousMillis = 0;
unsigned long ledinterval = 500;
unsigned long currentledMillis =0;
int ledState = false;

#if defined(ARDUINO_SAMD_NANO_33_IOT)
int ledPin = 13;
#elif defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
int ledPin = 2;
#endif

void setup() 
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);//LOG_LEVEL_SILENT - for disabling Serial prints.
  WiFi.begin(SSID,PASSWORD);            //Connecting to internet.
  delay(2000);

#if defined(ARDUINO_SAMD_NANO_33_IOT)
  String location = "arduino.location";   //Json structure of url in the web server
  String versionnum = "arduino.version";  //Json structure of the version number in the web server
#elif defined(ARDUINO_ARCH_ESP32) 
  String location = "esp32.location";
  String versionnum = "esp32.version";
#elif defined(ARDUINO_ARCH_ESP8266)
  String location = "esp8266.location";
  String versionnum = "esp8266.version";  
#endif 
/**
 *  initialize()- Setting up OTA library with JSON server url, 
 *  firmware version and location of json in webserver.
 **/
  ota.initialize(url, versionnum, location);  //setting up ota library with server parameters.

  #if defined(ARDUINO_SAMD_NANO_33_IOT) 
  if (ota.checkForUpdate(0,currentVersionNumber) == updateStatus::newUpdate)
#elif defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  if (ota.checkForUpdate(0) == updateStatus::newUpdate)
#endif
    ota.updateCode();           //Updating firmware if new version is available.

}

void loop() 
{
/**
 * checkForUpdate() - checks for if firmware is updated.
 **/
#if defined(ARDUINO_SAMD_NANO_33_IOT) 
  if (ota.checkForUpdate(600,currentVersionNumber) == updateStatus::newUpdate)
#elif defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266)
  if (ota.checkForUpdate(600) == updateStatus::newUpdate)
#endif
    ota.updateCode();           //Updating firmware if new version is available.

  currentledMillis = millis();
  if (currentledMillis - ledpreviousMillis > ledinterval) 
  {
    ledpreviousMillis = currentledMillis;
    ledState = (!ledState);
    digitalWrite(ledPin, ledState);
  }
}
