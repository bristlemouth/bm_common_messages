#pragma once
#include "cbor.h"
#include "power_reading_msg.h"
#include "sensor_header_msg.h"

namespace PowerReadingAveragesMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUMFIELDS = 12 + SensorHeaderMsg::NUM_FIELDS;

// static constexpr char

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingMsg::PowerReadingType_t power_reading_type;
  uint8_t status;
  uint32_t num_samples;
  double averaging_window_length_s;
  double voltage_v;
  double voltage_v_min;
  double voltage_v_max;
  double voltage_v_stdev;
  double current_a;
  double current_a_min;
  double current_a_max;
  double current_a_stdev;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PowerReadingAveragesMsg