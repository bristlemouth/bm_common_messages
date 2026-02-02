#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

namespace PowerReadingMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 4 + SensorHeaderMsg::NUM_FIELDS;

struct Data {
  SensorHeaderMsg::Data header;
  uint8_t power_reading_type;
  double voltage;
  double current;
  uint8_t status;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PowerReadingMsg