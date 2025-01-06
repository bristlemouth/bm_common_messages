#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

// For the PME dissolved oxygen sensor

namespace PmeDissolvedOxygenMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 4 + SensorHeaderMsg::NUM_FIELDS;

struct Data {
  SensorHeaderMsg::Data header;
  double temperature_deg_c;
  double do_mg_per_l;
  double quality;
  double do_saturation_pct;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PmeDissolvedOxygenMsg