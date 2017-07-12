#ifndef _WIFI_SECRETS_
#define _WIFI_SECRETS_

static const char host = "yourconfighostname";
static const char url = "yourapiurl";

static const int WIFI_MAX_SSID = 2;

static const char * wifiNetworks[WIFI_MAX_SSID][2] = {
  { "my_wifi1", "password1" },
  { "my_wifi2", "password2" }
};

static const char* ENTERPRISE_WIFI_SSID = "BigCorpWifi";
static const char* ENTERPRISE_WIFI_USER = "me";
static const char* ENTERPRISE_WIFI_PASS = "password";

#endif // _WIFI_SECRETS_

