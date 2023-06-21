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
  uint8_t version; // Protocol Version.
  uint8_t payload[0]; // Payload
} __attribute__ ((packed)) bm_common_pub_sub_header_t;

// Add pub sub data structures below


#ifdef __cplusplus
}
#endif
