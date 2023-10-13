#include "cbor.h"

namespace SysInfoSvcReplyMsg {

constexpr size_t NUM_FIELDS = 5;

struct Data {
  uint64_t node_id;
  uint32_t git_sha;
  uint32_t sys_config_crc;
  uint32_t app_name_strlen;
  char *app_name;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace SysInfoSvcReplyMsg
