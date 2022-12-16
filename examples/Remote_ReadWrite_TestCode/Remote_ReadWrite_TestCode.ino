#include "MQTTserver.h"

#define SSID "wifi_ssid"  //Repalce with your SSID
#define PASS "wifi_password"  //Repalce with your Password

int pinNo = 8; //Replace with your sensor connected pin number.

void mycallback(char *topic, byte *payload, unsigned int length) {
  char message[65];
  int i;
  Log.notice("Message arrived in topic: \n");
  Log.notice("%s\n",topic);
  Log.notice("Message:\t");
  for (i = 0; i < length; i++)
    message[i] = (char)payload[i];
  message[i] = '\0';
  Log.notice("%s\n", message);
  Log.notice("-----------------------\n");

  //For write the data 
  //Pin number expected to be two digits. Append a zero character, in case of single digit pin number. Example: 01
  //For writing Analog data you should follow [AW][pin][value] Example : "AW041"
  //For writing Digital data you should follow [DW][pin][value] Example : "DW041"
  MQTTserver::writePinData(payload);
}

void setup() {
  Serial.begin(115200);
  serverParam param;
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);  //LOG_LEVEL_SILENT - for disabling Serial prints.
  param.mqttServer = "iotsuite.io";     //server domain name or IP address.
  param.mqttUsername = "mqtt";          //server user name. provide NULL incase of open server.
  param.mqttPassword = "mqttpassword";          //server password.
  param.mqttPort = 1993;  //server port at which MQTT service is running.
  param.crt = NULL;       //provide NULL if no certificate is needed(only supported in esp32 and esp8266).

  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  mqtt.initialize(param, mycallback);
  mqtt.subscribe("/mqtt/test");  //Provide topic name


  mqtt.readPinData(pinNo, "/mqtt/test");
}

void loop() {
  mqtt.loop();
}
