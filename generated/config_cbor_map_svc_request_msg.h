#pragma once
#include <stdint.h>
#include "cbor.h"

#ifdef __cplusplus
namespace ConfigCborMapSvcRequestMsg {

constexpr size_t NUM_FIELDS = 1;

struct Data {
  uint32_t partition_id;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace ConfigCborMapSvcRequestMsg

extern "C" {
#endif

CborError config_cbor_map_svc_request_encode(CborEncoder * const map_encoder,
                               const uint32_t partition_id);

CborError config_cbor_map_svc_request_decode(                               uint32_t * const partition_id_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
