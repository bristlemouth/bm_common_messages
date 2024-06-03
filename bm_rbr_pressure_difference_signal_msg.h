#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

namespace BmRbrPressureDifferenceSignalMsg {

constexpr uint32_t VERSION = 1;

constexpr size_t NUM_FIELDS = SensorHeaderMsg::NUM_FIELDS + 2;

struct Data {
  SensorHeaderMsg::Data header;
  size_t num_samples;
  double *difference_signal;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace BmRbrPressureDifferenceSignalMsg
