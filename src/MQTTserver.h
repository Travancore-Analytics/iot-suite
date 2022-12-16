#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include <PubSubClient.h>

#if defined(ARDUINO_ARCH_ESP32) 
#include <WiFi.h>
#include <WiFiClientSecure.h>
#elif defined(ARDUINO_ARCH_ESP8266) 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#elif defined(ARDUINO_ARCH_SAMD)
#include <WiFiNINA.h>
#endif

struct serverParam
{
    char *mqttServer;
    char *mqttUsername;
    char *mqttPassword;
    uint16_t mqttPort;
    const char *crt;
};

typedef MQTT_CALLBACK_SIGNATURE;

class MQTTserver
{
public:
  void initialize(serverParam s,MQTT_CALLBACK_SIGNATURE);
  void loop();
  void subscribe(String topic);
  void publish(String topic,String msg);
  void readPinData(unsigned int pinNo,String topic);
  static void writePinData(byte *payload);
  
private:
   bool reconnect();

#if defined(ARDUINO_ARCH_ESP32)
  WiFiClientSecure espClient;
#endif
  WiFiClient client;
  PubSubClient mqttClient;
  serverParam param;
  long now,lastTime;
  String preTopic;
  callback mqttCallback;
};
extern class MQTTserver mqtt;
#endif
