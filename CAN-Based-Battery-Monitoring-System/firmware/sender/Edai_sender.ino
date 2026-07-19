#include <Wire.h>
#include "driver/twai.h"

// --- Voltage Divider (R1 = R2 = 10kΩ)
#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

void setup() {
  Serial.begin(115200);

  // Improve ADC accuracy
  analogSetAttenuation(ADC_11db);  // Recommended for inputs up to ~3.3V

  // Initialize CAN (TWAI)
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_22, GPIO_NUM_21, TWAI_MODE_NORMAL);  // TX=22, RX=21
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK && twai_start() == ESP_OK) {
    Serial.println("CAN Transmitter Ready");
  } else {
    Serial.println("CAN Init Failed");
    while (1);
  }
}

void loop() {
  // --- Read battery voltage
  int rawVoltage = analogRead(VOLTAGE_PIN);
  float measuredVoltage = rawVoltage * 3.3 / 4095.0;
  float batteryVoltage = measuredVoltage * 2.0;  // Account for 1:2 voltage divider

  // --- Read current sensor (ACS712)
  int rawCurrent = analogRead(CURRENT_PIN);
  float current = (rawCurrent * 3.3 / 4095.0 - 2.5) / 0.185;  // ACS712 5A

  // --- Print to Serial
  Serial.printf("Battery: %.2f V, Current: %.2f A\n", batteryVoltage, current);

  // --- Prepare CAN message
  twai_message_t message = {};
  message.identifier = 0x101;
  message.extd = 0;
  message.rtr = 0;
  message.data_length_code = 8;

  // Pack batteryVoltage and current into CAN message
  memcpy(&message.data[0], &batteryVoltage, 4);  // Bytes 0–3
  memcpy(&message.data[4], &current, 4);         // Bytes 4–7

  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("CAN Message Sent");
  } else {
    Serial.println("CAN Send Failed");
  }

delay(500);
}
