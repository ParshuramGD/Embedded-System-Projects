// CANHandler.h
#ifndef CAN_HANDLER_H
#define CAN_HANDLER_H

#include "driver/twai.h"
#include <cstdint>

struct CANFrame {
  uint32_t id;
  uint8_t  len;
  uint8_t  data[8];
};

class CANHandler {
public:
  bool begin();
  bool receive(CANFrame &frame, uint32_t timeout_ms=200);
};

#endif // CAN_HANDLER_H
