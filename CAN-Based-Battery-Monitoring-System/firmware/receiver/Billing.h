// Billing.h
#ifndef BILLING_H
#define BILLING_H

class Billing {
public:
  Billing(float rate, float threshold);
  void reset();
  void update(float voltage, float current, unsigned long now_ms);
  float getEnergy() const;
  float getCost()   const;
  bool  shouldNotify() const;

private:
  float rate_;
  float threshold_;
  float energy_kWh_;
  float cost_;
  unsigned long last_ms_;
  bool notified_;
};

#endif // BILLING_H
