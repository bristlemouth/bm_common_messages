#pragma once
#include <stdint.h>


namespace SensorHeaderMsg {

constexpr size_t NUM_FIELDS = 4;

struct Data {
  uint32_t version;
  uint64_t reading_time_utc_s;
  uint64_t reading_uptime_millis;
  uint64_t sensor_reading_time_s;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace SensorHeaderMsg

