#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

// For the Seapoint STM-S Turbidity Sensor

namespace BmTurbidityDataMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 2 + SensorHeaderMsg::NUM_FIELDS;

struct Data {
  SensorHeaderMsg::Data header;
  double s_signal;
  double r_signal;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace BmTurbidityDataMsg