#include "IoTSuite.h"

void setup() 
{
  Serial.begin(9600);
/**
 *initialize() - Setting up reachability library by 
 *providing required mode(BLE or AccessPoint),Time Interval(ms)
 *for advertising and device name. 
 **/
#if defined(ARDUINO_SAMD_NANO_33_IOT)|| defined(ARDUINO_ARCH_ESP32)
  reach.initialize(connectionType::accessPoint,30,"TEST BLE");
#elif defined(ARDUINO_ARCH_ESP8266)
  reach.initialize(30,"TEST BLE");
#else
  #error "BOARD NOT SUPPORTED"
#endif
  delay(1000);
  Serial.println("initialize completed.");
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
