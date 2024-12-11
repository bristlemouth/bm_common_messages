#pragma once
#include "cbor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SYS_INFO_REPLY_NUM_FIELDS 5

typedef struct {
  uint64_t node_id;
  uint32_t git_sha;
  uint32_t sys_config_crc;
  uint32_t app_name_strlen;
  char *app_name;
} SysInfoReplyData;

CborError sys_info_reply_encode(SysInfoReplyData *d, uint8_t *cbor_buffer, size_t size,
                                size_t *encoded_len);

CborError sys_info_reply_decode(SysInfoReplyData *d, const uint8_t *cbor_buffer, size_t size);

#ifdef __cplusplus
}
#endif
