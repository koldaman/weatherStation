#include "Arduino.h"

#include <WeatherWebServer.h>
#include <CustomWiFiManager.h>
#include <CustomWiFiClient.h>
#include <DHT.h>
#include <LedIndicator.h>

// zvoleni typu senzoru
#define DHTTYPE DHT22      // DHT 22 (AM2302), AM2321

// inicializace DHT sensoru
const int DHTPin = 5;   // D1
DHT dht(DHTPin, DHTTYPE);

const int LED_INDICATOR_PIN = 14;   // D4

const long DEEP_SLEEP_TIME = 1*60*1000000; // 1 minuta

// HTTP server
WeatherWebServer server(80);

// zasilani informaci do cloudu
CustomWiFiClient client;

float temp;
float hum;

long previousTime = 0;

void readAndSend() {
   Serial.println("Reading data from sensor...");
   temp = dht.readTemperature(false); // false - teplota v Celsiich, true - ve fahreheitech
   hum = dht.readHumidity();

   // logovani posilanych dat
   Serial.print("Temperature: ");
   Serial.print(temp);
   Serial.print("\tHumidity: ");
   Serial.println(hum);

   client.sendData(temp, hum);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  LedIndicator wifiLedIndicator;
  wifiLedIndicator.init(LED_INDICATOR_PIN);

  CustomWiFiManager::start(&wifiLedIndicator);

  delay(100);
  dht.begin();

  server.init();
  client.init();
  readAndSend();

  ESP.deepSleep(DEEP_SLEEP_TIME, WAKE_RF_DEFAULT);
}

void loop() {
  // netreba implementovat, vse je v metode setup()
}
