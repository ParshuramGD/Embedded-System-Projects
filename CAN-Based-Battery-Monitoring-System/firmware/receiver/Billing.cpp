// Billing.cpp
#include "Billing.h"
#include <Arduino.h>
Billing::Billing(float rate, float threshold)
 : rate_(rate), threshold_(threshold), energy_kWh_(0), cost_(0),
   last_ms_(millis()), notified_(false) {}

void Billing::reset() {
  energy_kWh_ = cost_ = 0;
  last_ms_ = millis();
  notified_ = false;
}

void Billing::update(float V, float I, unsigned long now_ms) {
  float dt_h = (now_ms - last_ms_) / 3600000.0f;
  last_ms_ = now_ms;
  energy_kWh_ += V * I * dt_h;
  cost_       = energy_kWh_ * rate_;
  if (cost_ >= threshold_) notified_ = true;
}

float Billing::getEnergy()   const { return energy_kWh_; }
float Billing::getCost()     const { return cost_; }
bool  Billing::shouldNotify() const { return notified_; }
