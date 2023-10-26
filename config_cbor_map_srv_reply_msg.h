#pragma once
#include "cbor.h"

namespace ConfigCborMapSrvReplyMsg {

constexpr size_t NUM_FIELDS = 4;

struct Data {
    uint32_t partition_id;
    bool success;
    uint32_t cbor_encoded_map_len;
    uint8_t cbor_data[0];
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace ConfigCborMapSrvReplyMsg
