#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

// For the wipe data from the PME dissolved oxygen sensor

namespace PmeWipeMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 6 + SensorHeaderMsg::NUM_FIELDS;

struct Data {
  SensorHeaderMsg::Data header;
  double wipe_time_sec;
  double start1_mA;
  double avg_mA;
  double start2_mA;
  double final_mA;
  double rsource;
};

CborError encode(Data &w, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &w, const uint8_t *cbor_buffer, size_t size);

} // namespace PmeWipeMsg