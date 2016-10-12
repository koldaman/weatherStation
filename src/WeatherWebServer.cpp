/**************************************************************
   Zabaleni funkcionality pro WebServer
 **************************************************************/
#include "Arduino.h"

#include "WeatherWebServer.h"
#include "ESP8266WebServer.h"

WeatherWebServer::WeatherWebServer(const int port) {
   ESP8266WebServer _server(port);
}

void WeatherWebServer::handleClient(float temp, float hum) {
   _temp = temp;
   _hum = hum;
   _server.handleClient();
}

void WeatherWebServer::switchPin(uint8_t pin, int state) {
   Serial.println("Switching state");
   digitalWrite(pin, state);
}

void WeatherWebServer::handleIndex() {
    String args = "";
    for (uint8_t i = 0; i < _server.args(); i++) {
       args += _server.argName(i);
       args += "=";
       args += _server.arg(i);
       if (_server.argName(i) == "switch") {
          if (_server.arg(i) == "0") {
             switchPin(BUILTIN_LED, false);
          } else if (_server.arg(i) == "1") {
             switchPin(BUILTIN_LED, true);
          }
        } else {
          Serial.println("unknown argument! ");
        }
     }

    String html = "\
    <!doctype html>\
    <html class='no-js' lang=''>\
        <head>\
            <meta charset='utf-8'>\
            <meta http-equiv='x-ua-compatible' content='ie=edge'>\
            <title>Meteostanice</title>\
            <meta name='description' content=''>\
            <meta name='viewport' content='width=device-width, initial-scale=1'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/normalize/5.0.0/normalize.min.css'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/css/font-awesome.min.css'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/fonts/fontawesome-webfont.eot'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/fonts/fontawesome-webfont.svg'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/fonts/fontawesome-webfont.ttf'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/fonts/fontawesome-webfont.woff'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/fonts/fontawesome-webfont.woff2'>\
            <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.6.3/fonts/FontAwesome.otf'>\
            <link rel='stylesheet' href='css/main.css'>\
            <script src='https://cdnjs.cloudflare.com/ajax/libs/modernizr/2.8.3/modernizr.min.js'></script>\
    		<style>\
          body {\
             background-color: rgba(0,0,0,.05);\
          }\
          h1 {\
             color: rgba(255,255,255,.9);\
             text-shadow: rgba(0,0,0,.2) 0px -1px 0px;\
             background-color: #2d353f;\
             padding: 1em;\
             margin: 0;\
             font-size: 2em;\
             text-align: center;\
          }\
          .container {\
             text-align: center;\
             padding: 0 3em;\
          }\
          .block {\
             background-color: #fff;\
             margin: 1em 0;\
             padding: 1em 0;\
             border: 1px lightgray solid;\
             width: 300px;\
             display: inline-block;\
          }\
          .block .fa {\
             color: lightgray;\
             float: left;\
             margin: 0.1em 0.6em;\
          }\
          .block .header {\
             color: lightgray;\
             font-size: 2em;\
             float: left;\
          }\
          .block .value {\
             color: #2d353f;\
             padding-top: 1em;\
             font-size: 3em;\
             text-align: center;\
          }\
          .switch {\
             padding: 0.5em;\
             font-size: 2em;\
             text-align: center;\
          }\
          @media only screen and (max-width: 768px) {\
             .block {\
                width: 100%;\
             }\
          }\
       </style>\
    </head>\
    <body>\
         <h1 class='header'>Meteostanice</h1>\
         <div class='container'>\
           <div class='block'>\
                <i class='fa fa-sun-o fa-2x'></i> <div class='header'>Teplota</div>\
               <div class='value'>\
    ";
    html += _temp;
    html += "\
               &deg;C</div>\
            </div>\
            <div class='block'>\
               <i class='fa fa-tint fa-2x'></i> <div class='header'>Vlhkost</div>\
               <div class='value'>\
    ";
    html += _hum;
    html += "\
                %</div>\
            </div>\
         </div>\
    ";
    html += "\
    </body>\
    </html>\
    ";

   _server.send(200, "text/html", html);
}

void WeatherWebServer::init() {
   std::function<void ()> handleIndex = [&]{ WeatherWebServer::handleIndex(); };
   _server.on("/", handleIndex);
   _server.begin();
   Serial.println("HTTP server started");
}
