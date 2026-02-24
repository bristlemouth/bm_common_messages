#pragma once
#include "cbor.h"
#include "power_reading_msg.h"
#include "sensor_header_msg.h"

namespace PowerBatteryMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 12 + SensorHeaderMsg::NUM_FIELDS;
static constexpr char CHARGE_AH[] = "charge_ah";
static constexpr char CAPACITY_AH[] = "capacity_ah";
static constexpr char PERCENTAGE[] = "percentage";
static constexpr char BATTERY_STATUS[] = "battery_status";
static constexpr char BATTERY_HEALTH[] = "battery_health";
static constexpr char NUM_CELLS[] = "num_cells";
static constexpr char CELL_VOLTAGE_V[] = "cell_voltage_v";
static constexpr char CELL_TEMPERATURE_C[] = "cell_temperature_c";

// Following the linux power supply enum: https://github.com/torvalds/linux/blob/master/include/linux/power_supply.h
typedef enum PowerBatteryStatus : uint8_t {
  STATUS_UNKNOWN = 0,
  CHARGING = 1,
  DISCHARGING = 2,  // battery is providing power to the load, current flowing out
  NOT_CHARGING = 3, // current is not flowing in, could be flowing out or not flowing
  FULL = 4,
} PowerBatteryStatus_t;

typedef enum PowerBatteryHealth : uint8_t {
  HEALTH_UNKNOWN = 0,
  GOOD = 1,
  OVERHEAT = 2,
  DEAD = 3,
  COLD = 4,
  UNSPEC_FAILURE = 5,
} PowerBatteryHealth_t;

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingMsg::PowerReadingType_t power_reading_type;
  uint8_t status;
  double voltage_v;
  double current_a;
  double charge_ah;
  double capacity_ah;
  double percentage;
  PowerBatteryStatus_t battery_status;
  PowerBatteryHealth_t battery_health;
  uint8_t num_cells;
  double *cell_voltage_v;
  double *cell_temperature_c;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PowerBatteryMsg
