#pragma once
#include "cbor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_CBOR_MAP_REPLY_NUM_FIELDS 5

typedef struct {
  uint64_t node_id;
  uint32_t partition_id;
  bool success;
  uint32_t cbor_encoded_map_len;
  uint8_t *cbor_data;
} ConfigCborMapReplyData;

CborError config_cbor_map_reply_encode(ConfigCborMapReplyData *d, uint8_t *cbor_buffer,
                                       size_t size, size_t *encoded_len);

CborError config_cbor_map_reply_decode(ConfigCborMapReplyData *d, const uint8_t *cbor_buffer,
                                       size_t size);

#ifdef __cplusplus
}
#endif
