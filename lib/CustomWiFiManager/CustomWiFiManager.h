/**************************************************************
Zabaleni funkcionality pro WiFiManager (zapamatovani
posledniho nastaveni pripojeni k WiFi vcetne prenastaveni)
**************************************************************/

#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <LedIndicator.h>

class CustomWiFiManager {
public:
   static void start(LedIndicator* indicator);
private:
  static WiFiManager _wifiManager;
  static LedIndicator* _indicator;
  static void configModeCallback(WiFiManager *myWiFiManager);
};
