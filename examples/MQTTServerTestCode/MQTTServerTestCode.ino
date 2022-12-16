#include "IoTSuite.h"

#define SSID "wifi_ssid"
#define PASS "wifi_password"

void mycallback(char *topic, byte *payload, unsigned int length) 
{
  char message[65];
  int i;
  Log.notice("Message arrived in topic: \n");
  Log.notice("%s\n",topic);
  Log.notice("Message:\n");
  for (i = 0; i < length; i++) 
      message[i] = (char)payload[i];
  message[i] = '\0';
  Log.notice("%s\n",message);
  Log.notice("-----------------------\n");
}

void setup()
{
  Serial.begin(115200);
  serverParam param;

  Log.begin(LOG_LEVEL_VERBOSE, &Serial);//LOG_LEVEL_SILENT - for disabling Serial prints.
  param.mqttServer = "iotsuite.io";     //server domain name or IP address.
  param.mqttUsername = "mqtt";          //server user name. provide NULL incase of open server.
  param.mqttPassword = "mqttpassword";          //server password.
  param.mqttPort = 1993;                //server port at which MQTT service is running.
  param.crt = NULL;                     //provide NULL if no certificate is needed(only supported in esp32 and esp8266).
  
  WiFi.begin(SSID,PASS);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
  }
  mqtt.initialize(param,mycallback);
  mqtt.subscribe("/mqtt/test");          //Provide topic name 
}

void loop()
{
  mqtt.loop();
}
