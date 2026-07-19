
// --- a/src/BlynkHandler.cpp
// +++ b/src/BlynkHandler.cpp
// @@ -1,3 +1,6 @@
#include "Config.h"            // <— Must come first!
#include <WiFi.h>              // ESP32 WiFi
 #include <BlynkSimpleEsp32.h>   // Blynk
 #include "BlynkHandler.h"
#include <Arduino.h>

void BlynkHandler::begin() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect();
}

void BlynkHandler::run() { Blynk.run(); }

void BlynkHandler::sendVoltage(float V) { Blynk.virtualWrite(VP_VOLTAGE, V); }
void BlynkHandler::sendCurrent(float I) { Blynk.virtualWrite(VP_CURRENT, I); }
void BlynkHandler::sendEnergy(float kWh) { Blynk.virtualWrite(VP_ENERGY, kWh); }
void BlynkHandler::sendBill(float bill)   { Blynk.virtualWrite(VP_BILL, bill); }
void BlynkHandler::sendRate(float rate)   { Blynk.virtualWrite(VP_RATE, rate); }
void BlynkHandler::sendSoC(float soc)     { Blynk.virtualWrite(VP_SOC, soc); }

void BlynkHandler::notifyHighBill() {
  Blynk.logEvent("high_bill", "Energy Bill Exceeded ₹10");
}
