#pragma once
#include <stdint.h>
#include "cbor.h"

#ifdef __cplusplus
namespace SysInfoSvcReplyMsg {

constexpr size_t NUM_FIELDS = 5;

struct Data {
  uint64_t node_id;
  uint32_t git_sha;
  uint32_t sys_config_crc;
  uint32_t app_name_strlen;
  char* app_name;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace SysInfoSvcReplyMsg

extern "C" {
#endif

CborError sys_info_svc_reply_encode(CborEncoder * const map_encoder,
                               const uint64_t node_id,
                               const uint32_t git_sha,
                               const uint32_t sys_config_crc,
                               const uint32_t app_name_strlen,
                               const char* app_name);

CborError sys_info_svc_reply_decode(                               uint64_t * const node_id_p,
                               uint32_t * const git_sha_p,
                               uint32_t * const sys_config_crc_p,
                               uint32_t * const app_name_strlen_p,
                               char ** const app_name_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
