/**************************************************************
  Zabaleni funkcionality pro WiFiManager (pamatuje si udaje
  posledni pripojene WiFi site vcetne moznosti menit tento udaj)
 **************************************************************/
#include "Arduino.h"

#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <CustomWiFiManager.h>
#include <LedIndicator.h>

LedIndicator* CustomWiFiManager::_indicator;

// spousti se behem rezimu konfigurace
void CustomWiFiManager::configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  // vypis konfiguracni SSID
  Serial.println(myWiFiManager->getConfigPortalSSID());
  // konfiguracni mod, blikej rychleji
  if (CustomWiFiManager::_indicator != NULL) {
    CustomWiFiManager::_indicator->start(0.2);
  }
}

void CustomWiFiManager::start(LedIndicator* indicator) {
  CustomWiFiManager::_indicator = indicator;
  // pokus o pripojeni k predchozi WiFi
  if (CustomWiFiManager::_indicator != NULL) {
    CustomWiFiManager::_indicator->start(0.6);
  }

  WiFiManager wifiManager;

  //reset settings - for testing
  // wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(&CustomWiFiManager::configModeCallback);

  IPAddress _ip = IPAddress(10, 0, 0, 1);
  IPAddress _gw = IPAddress(10, 0, 0, 2);
  IPAddress _sn = IPAddress(255, 255, 255, 0);

  wifiManager.setAPStaticIPConfig(_ip, _gw, _sn);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "ESPAutoConnectAP" (with password) and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("ESPAutoConnectAP", "esppassword")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.print("connected to SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("on local ip: ");
  Serial.println(WiFi.localIP());

  if (CustomWiFiManager::_indicator != NULL) {
    CustomWiFiManager::_indicator->stop();
  }
}
