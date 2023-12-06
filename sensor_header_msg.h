#pragma once
#include <stdint.h>
#include "cbor.h"

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

