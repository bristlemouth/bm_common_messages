#pragma once
#include "cbor.h"

namespace DeviceTestSvcRequestMsg {

constexpr size_t NUM_FIELDS = 2;

struct Data {
  uint32_t data_len;
  uint8_t *data;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace DeviceTestSvcRequestMsg
