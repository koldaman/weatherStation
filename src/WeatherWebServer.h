/**************************************************************
Zabaleni funkcionality pro WebServer
**************************************************************/

#include <ESP8266WebServer.h>

class WeatherWebServer {
public:
   WeatherWebServer(const int port);
   void init();
   void handleClient(float temp, float hum);
private:
   ESP8266WebServer _server;
   float            _temp;
   float            _hum;
   void             handleIndex();
   void             switchPin(uint8_t pin, int state);
};
