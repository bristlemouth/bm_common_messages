#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

namespace BmRbrPressureDifferenceSignalMsg {

constexpr uint32_t VERSION = 1;

constexpr size_t NUM_FIELDS = SensorHeaderMsg::NUM_FIELDS + 6;

struct Data {
  SensorHeaderMsg::Data header;
  uint32_t sequence_num;
  uint32_t total_samples;
  size_t num_samples;
  double residual_0;
  double residual_1;
  double *difference_signal;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace BmRbrPressureDifferenceSignalMsg
