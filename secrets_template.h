#ifndef _WIFI_SECRETS_
#define _WIFI_SECRETS_

static const char* config_host = "yourconfighostname";
static const char* config_url = "yourconfigapiurl";

static const char* slack_host = "yourslackhostname";
static const char* slack_url = "yourslackapiurl";

static const int WIFI_MAX_SSID = 2;

static const char * wifiNetworks[WIFI_MAX_SSID][2] = {
  { "my_wifi1", "password1" },
  { "my_wifi2", "password2" }
};

static const char* ENTERPRISE_WIFI_SSID = "BigCorpWifi";
static const char* ENTERPRISE_WIFI_USER = "me";
static const char* ENTERPRISE_WIFI_PASS = "password";

#endif // _WIFI_SECRETS_

