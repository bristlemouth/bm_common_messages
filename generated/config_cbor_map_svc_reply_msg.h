#pragma once
#include <stdint.h>
#include "cbor.h"

#ifdef __cplusplus
namespace ConfigCborMapSvcReplyMsg {

constexpr size_t NUM_FIELDS = 5;

struct Data {
  uint64_t node_id;
  uint32_t partition_id;
  bool success;
  uint32_t cbor_encoded_map_len;
  uint8_t* cbor_data;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace ConfigCborMapSvcReplyMsg

extern "C" {
#endif

CborError config_cbor_map_svc_reply_encode(CborEncoder * const map_encoder,
                               const uint64_t node_id,
                               const uint32_t partition_id,
                               const bool success,
                               const uint32_t cbor_encoded_map_len,
                               const uint8_t* cbor_data);

CborError config_cbor_map_svc_reply_decode(                               uint64_t * const node_id_p,
                               uint32_t * const partition_id_p,
                               bool * const success_p,
                               uint32_t * const cbor_encoded_map_len_p,
                               uint8_t* * const cbor_data_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
