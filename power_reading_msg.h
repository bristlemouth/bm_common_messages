#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

namespace PowerReadingMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 4 + SensorHeaderMsg::NUM_FIELDS;

static constexpr char POWER_READING_TYPE[] = "power_reading_type";
static constexpr char VOLTAGE_V[] = "voltage_v";
static constexpr char CURRENT_MA[] = "current_ma";
static constexpr char STATUS[] = "status";

typedef enum PowerReadingType {
  SOURCE,
  LOAD,
  MONITOR,
} PowerReadingType_t;

typedef enum StatusFlags {
  OKAY = 0,
  UNDERVOLTAGE =  (1 << 0),
  OVERVOLTAGE = (1 << 1),
  UNDERCURRENT = (1 << 2),
  OVERCURRENT = (1 << 3),
} StatusFlags_t;

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingType_t power_reading_type;
  double voltage_v;
  double current_ma;
  uint64_t status;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PowerReadingMsg