#include "Wifi_module.h"

Wifi::Wifi(Led* my_led) {
  led_pntr = my_led;
}

void Wifi::setup() {
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED && attempts < MAX_ATTEMPTS) {
    //wifiEnterpriseSetup();
    Serial.print(F("\nConnecting to regular wifi: "));
    wifi_station_set_wpa2_enterprise_auth(0);
    wifiIndex = 0;
    while (WiFi.status() != WL_CONNECTED && wifiIndex < WIFI_MAX_SSID) {
      wifiRegularSetup();
    }
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("\nWiFi connected"));  
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());
    led_pntr->signalWifiOk();
  } else {
    Serial.println(F("\nFailed to connect to any of the specified wifi networks."));
    Serial.println(F("Try again!"));
    led_pntr->signalWifiFailure();
  }
}

void Wifi::resetCounters() {
  attempts = 0;
  wifiIndex = 0;
  wifiTimer = 0;
  //retryCounter = 0;
  enterpriseRetryCounter = 0;
}

void Wifi::wifiRegularSetup() {
  if (wifiTimer == 0) {
    Serial.println(wifiNetworks[wifiIndex][0]);
    WiFi.begin(wifiNetworks[wifiIndex][0], wifiNetworks[wifiIndex][1]);
  }
  delay(500);
  Serial.print(F("."));
  wifiTimer++;
  if (wifiTimer >= WIFI_TIMEOUT) {
    WiFi.disconnect();
    Serial.print(F("\nTrying next ssid: "));
    wifiIndex++;
    //retryCounter++;
    wifiTimer = 0;
  }
}

void Wifi::wifiEnterpriseSetup() {
  Serial.print(F("\n\nConnecting to enterprise wifi: "));
  Serial.println(F("Statoil Approved"));
  ip_info info;
  wifi_get_ip_info(0, &info);
  
  wifi_station_disconnect();
  wifi_set_opmode(STATION_MODE);

  struct station_config wifi_config;

  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ENTERPRISE_WIFI_SSID);

  if (!wifi_station_set_config(&wifi_config)) {
    Serial.println(F("Set config failed."));
  }

  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_cert_key();
  wifi_station_clear_enterprise_ca_cert();
//  wifi_station_set_enterprise_identity("JOE@statoil.com");
  wifi_station_set_wpa2_enterprise_auth(1);
  
  if (!wifi_station_set_enterprise_username((uint8*)ENTERPRISE_WIFI_USER, strlen(ENTERPRISE_WIFI_USER))==0) {
    Serial.println(F("Set username failed."));
  }
  
  if (!wifi_station_set_enterprise_password((uint8*)ENTERPRISE_WIFI_PASS, strlen(ENTERPRISE_WIFI_PASS))==0) {
    Serial.println(F("Set password failed."));
  }

  if (!wifi_station_connect()) {
    Serial.println(F("Connection failed."));
  }

  // Wait for connection AND IP address from DHCP
  while (info.ip.addr == 0 && enterpriseRetryCounter < WIFI_TIMEOUT) {
  //while (WiFi.status() != WL_CONNECTED && enterpriseRetryCounter < WIFI_TIMEOUT) {
    ESP.wdtFeed();
    Serial.print(F("."));
    delay(1000);
    wifi_get_ip_info(0, &info);
    enterpriseRetryCounter++;
  }
  enterpriseRetryCounter = 0;
  WiFi.disconnect();
  Serial.println("");
}


