#include "Config.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "canhandler.h"
#include "blynk.h"
#include "Billing.h"

CANHandler   can;
BlynkHandler blynk;
Billing      billing(BILLING_RATE, HIGH_BILL_THRESHOLD);

float lastVoltage = 0, lastCurrent = 0, lastSoC = 0;

void setup() {
  Serial.begin(115200);
  blynk.begin();
  if (!can.begin()) {
    Serial.println("CAN init failed"); while(true) delay(1000);
  }
  billing.reset();
}

void loop() {
  blynk.run();

  CANFrame frame;
  if (can.receive(frame)) {
    if (frame.id == CAN_ID_VI && frame.len == 8) {
      memcpy(&lastVoltage, &frame.data[0], 4);
      memcpy(&lastCurrent, &frame.data[4], 4);

      billing.update(lastVoltage, lastCurrent, millis());
      blynk.sendVoltage(lastVoltage);
      blynk.sendCurrent(lastCurrent);
      blynk.sendEnergy(billing.getEnergy());
      blynk.sendBill(billing.getCost());
      blynk.sendRate(BILLING_RATE);

      Serial.printf("V=%.2f I=%.2f E=%.4f kWh Bill=₹%.2f\n",
                    lastVoltage, lastCurrent,
                    billing.getEnergy(), billing.getCost());

      if (billing.shouldNotify()) {
        blynk.notifyHighBill();
      }
    }
    else if (frame.id == CAN_ID_SOC && frame.len == 4) {
      memcpy(&lastSoC, &frame.data[0], 4);
      blynk.sendSoC(lastSoC);
      Serial.printf("SoC=%.1f%%\n", lastSoC);
    }
  }
  delay(500);
}
