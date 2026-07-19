// BlynkHandler.h
#ifndef BLYNK_HANDLER_H
#define BLYNK_HANDLER_H

class BlynkHandler {
public:
  void begin();
  void run();
  void sendVoltage(float V);
  void sendCurrent(float I);
  void sendEnergy(float kWh);
  void sendBill(float bill);
  void sendRate(float rate);
  void sendSoC(float soc);
  void notifyHighBill();
};

#endif // BLYNK_HANDLER_H
