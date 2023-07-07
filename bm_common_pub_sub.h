#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BM_COMMON_PUB_SUB_VERSION
#define BM_COMMON_PUB_SUB_VERSION (1)
#endif // BM_COMMON_PUB_SUB_VERSION

// Add data structures published through pub sub must have this header.
typedef struct {
  uint8_t type; // Type of data.
  uint8_t version; // Protocol Version.
  uint8_t payload[0]; // Payload
} __attribute__ ((packed)) bm_common_pub_sub_header_t;

// Add pub sub data structures below

typedef struct {
    uint64_t utc_us; // UTC in nanoseconds.
} __attribute__ ((packed)) bm_common_pub_sub_utc_t;

// This is the payload for a printf/fprintf publication
typedef struct {
  uint64_t target_node_id;
  uint16_t fname_len;
  uint16_t data_len;
  uint8_t fnameAndData[0];
} __attribute__((packed)) bm_print_publication_t;

#ifdef __cplusplus
}
#endif
