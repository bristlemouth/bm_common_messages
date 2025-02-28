#ifndef __POWER_INFO_REPLY_MSG_H__
#define __POWER_INFO_REPLY_MSG_H__

#include "cbor.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define power_info_reply_msg_num_fields (3)

typedef struct {
  uint32_t total_on_s;
  uint32_t remaining_on_s;
  uint32_t upcoming_off_s;
} PowerInfoReplyData;

CborError power_info_reply_encode(PowerInfoReplyData *d, uint8_t *cbor_buffer,
                                  size_t size, size_t *encoded_len);
CborError power_info_reply_decode(PowerInfoReplyData *d,
                                  const uint8_t *cbor_buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif
