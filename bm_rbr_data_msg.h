#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

namespace BmRbrDataMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 3 + SensorHeaderMsg::NUM_FIELDS;

typedef enum SensorType {
  UNKNOWN,
  TEMPERATURE,
  PRESSURE,
  PRESSURE_AND_TEMPERATURE,
} SensorType_t;

struct Data {
  SensorHeaderMsg::Data header;
  SensorType_t sensor_type;
  double temperature_deg_c;
  double pressure_deci_bar;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace BmRbrDataMsg