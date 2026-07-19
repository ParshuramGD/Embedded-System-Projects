// CANHandler.cpp
#include "CANHandler.h"
#include <Arduino.h>
bool CANHandler::begin() {
  twai_general_config_t gcfg = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_22, GPIO_NUM_21, TWAI_MODE_NORMAL);
  twai_timing_config_t  tcfg = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t  fcfg = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  return (twai_driver_install(&gcfg, &tcfg, &fcfg) == ESP_OK)
      && (twai_start() == ESP_OK);
}

bool CANHandler::receive(CANFrame &frame, uint32_t timeout_ms) {
  twai_message_t msg;
  if (twai_receive(&msg, pdMS_TO_TICKS(timeout_ms)) == ESP_OK) {
    frame.id  = msg.identifier;
    frame.len = msg.data_length_code;
    memcpy(frame.data, msg.data, 8);
    return true;
  }
  return false;
}
