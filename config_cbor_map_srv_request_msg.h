#pragma once
#include "cbor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_CBOR_MAP_REQUEST_NUM_FIELDS 1

typedef struct {
  uint32_t partition_id;
} ConfigCborMapRequestData;

CborError config_cbor_map_request_encode(ConfigCborMapRequestData *d, uint8_t *cbor_buffer,
                                         size_t size, size_t *encoded_len);

CborError config_cbor_map_request_decode(ConfigCborMapRequestData *d,
                                         const uint8_t *cbor_buffer, size_t size);

#ifdef __cplusplus
}
#endif
