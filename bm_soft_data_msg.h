#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

namespace BmSoftDataMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 1 + SensorHeaderMsg::NUM_FIELDS;

struct Data {
  SensorHeaderMsg::Data header;
  double temperature_deg_c;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace BmSoftDataMsg