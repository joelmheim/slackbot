#ifndef _Wifi_
#define _Wifi_

#include <ESP8266WiFi.h>

#include "secrets.h"
#include "LED_module.h"

extern "C" {
  #include "user_interface.h"
  #include "wpa2_enterprise.h"
}


///////////////////////////////////////

class Wifi {
public:
  Wifi(Led* my_led);
  void setup();
  void resetCounters();
private:
  Led* led_pntr;
  void wifiEnterpriseSetup();
  void wifiRegularSetup();
  const int MAX_ATTEMPTS = 2;
  const int WIFI_TIMEOUT = 15;
  //const int MAX_RETRIES = WIFI_MAX_SSID * MAX_ATTEMPTS;
  int attempts = 0;
  int wifiIndex = 0;
  int wifiTimer = 0;
  //int retryCounter = 0;
  int enterpriseRetryCounter = 0;
};

#endif // _Wifi_
