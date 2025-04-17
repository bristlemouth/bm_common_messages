#pragma once
#include <stdint.h>
#include "cbor.h"

#ifdef __cplusplus
namespace SensorHeaderMsg {

constexpr size_t NUM_FIELDS = 4;

struct Data {
  uint32_t version;
  uint64_t reading_time_utc_ms;
  uint64_t reading_uptime_millis;
  uint64_t sensor_reading_time_ms;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace SensorHeaderMsg

extern "C" {
#endif

CborError sensor_header_encode(CborEncoder * const map_encoder,
                               const uint32_t version,
                               const uint64_t reading_time_utc_ms,
                               const uint64_t reading_uptime_millis,
                               const uint64_t sensor_reading_time_ms);

CborError sensor_header_decode(uint32_t * const version_p,
                               uint64_t * const reading_time_utc_ms_p,
                               uint64_t * const reading_uptime_millis_p,
                               uint64_t * const sensor_reading_time_ms_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
