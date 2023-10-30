#pragma once
#include "cbor.h"

namespace ConfigCborMapSrvRequestMsg {

constexpr size_t NUM_FIELDS = 1;

struct Data {
    uint32_t partition_id;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace ConfigCborMapSrvRequestMsg
