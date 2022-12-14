#include "IoTSuite.h"

void setup() 
{
  Serial.begin(9600);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial); //LOG_LEVEL_SILENT - for disabling Serial prints.
/**
 *initialize() - Setting up reachability library by 
 *providing required mode(BLE or AccessPoint),Time Interval(ms)
 *for advertising and device name. 
 **/
#if defined(ARDUINO_SAMD_NANO_33_IOT)|| defined(ARDUINO_ARCH_ESP32)
  reach.initialize(connectionType::accessPoint,90,"TEST BLE");
#elif defined(ARDUINO_ARCH_ESP8266)
  reach.initialize(90,"TEST BLE");
#else
  #error "BOARD NOT SUPPORTED"
#endif
  delay(1000);
  Log.notice("initialize completed.\n");
}

void loop() 
{
/**
 * loop()-Handles all the ble callbacks,AccessPoint 
 * callbacks and checks for connection status as per
 * inteval provided while initilalizing.
 */
  reach.loop();  
}
