#pragma once
#include "bm_messages_helper.h"

#ifdef __cplusplus
extern "C" {
#endif


#define METRICS_REPLY_VERSION 1
#define METRICS_REPLY_NUM_FIELDS 4 // mv, node, up, data

typedef struct {
  const char *key;
  const BmEncoderTableEntry_t *fields; // filled LUT of flat metric fields
  size_t num_fields; // number of valid entries in fields
} MetricsComponent;

typedef struct {
  uint8_t version;
  uint64_t node_id;
  uint32_t uptime_ms;
  const MetricsComponent *components;
  size_t num_components;
} MetricsReplyData;

typedef struct {
  const char *key;
  const BmDecodeTableEntry_t *fields;
  size_t num_fields;
} MetricsComponentDecode;

typedef struct {
  uint8_t *version;
  uint64_t *node_id;
  uint32_t *uptime_ms;
  const MetricsComponentDecode *components;
  size_t num_components;
} MetricsReplyDecode;

CborError metrics_reply_encode(const MetricsReplyData *d, uint8_t *cbor_buffer,
                               size_t size, size_t *encoded_len);

CborError metrics_reply_decode(const uint8_t *cbor_buffer, size_t size,
                               const MetricsReplyDecode *out);

#ifdef __cplusplus
}
#endif
