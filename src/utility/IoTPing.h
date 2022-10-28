#if defined (ARDUINO_ARCH_ESP32) || (ARDUINO_ARCH_ESP8266)

#ifndef IOT_PING_H
#define IOT_PING_H

#include <Arduino.h>

#ifdef ARDUINO_ARCH_ESP32
  #include <WiFi.h>
#elif ARDUINO_ARCH_ESP8266
  #include <ESP8266WiFi.h>
  extern "C"
  {
    #include <ping.h>
  }
#endif

typedef void(*ping_recv_ptr)(void* arg, void *pdata);
typedef void(*ping_sent_ptr)(void* arg, void *pdata);

#ifdef ARDUINO_ARCH_ESP32
struct ping_opts
{
  uint32_t count;
  uint32_t ip;
  uint32_t coarse_time;
  ping_recv_ptr recv_function;
  ping_sent_ptr sent_function;
};

struct response
{
  uint32_t total_count;
  uint32_t seqno;
  uint32_t timeout_count;
  uint32_t bytes;
  uint32_t total_bytes;
  float resp_time;
  float total_time;
  int8_t  error_code;
};

#endif

class IoTPing
{

public:
  IoTPing();
  bool ping(IPAddress dest, byte count =5);
  bool ping(const char *host, byte count = 5);

protected:
  static void _ping_recv_cb(void *opt, void *pdata);

#ifdef ARDUINO_ARCH_ESP32
  ping_opts _options;
#elif ARDUINO_ARCH_ESP8266
  ping_option _options;
#endif

  static byte _expected_count, _errors, _success;
  static float _avg_time;

  static uint32_t received;
  static float min_time;
  static uint32_t transmitted;
  static float max_time ;
  static float mean_time ;
  static float last_mean_time;
  static float var_time ;

private:
  static uint16_t ping_seq_num;
  static uint8_t stopped;

#ifdef ARDUINO_ARCH_ESP32
  bool start_ping(struct ping_opts *ping_opt);
  void ping(const char *name, int count, int interval, int size, int timeout);
  bool start_ping(IPAddress adr, int count, int interval, int size, int timeout, struct ping_opts *ping_o);

  void recv_ping(int s);
  void ping_prepare_echo(struct icmp_echo_hdr *iecho, uint16_t len);
  int send_ping(int s, ip4_addr_t *addr, int size);
#endif

};
extern class IoTPing iotPing;
#endif
#endif