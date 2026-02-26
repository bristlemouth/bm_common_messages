#pragma once
#include "cbor.h"
#include "power_reading_msg.h"
#include "sensor_header_msg.h"

namespace PowerSolarReadingMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 9 + SensorHeaderMsg::NUM_FIELDS;
static constexpr char MPP_POSITION[] = "mpp_position";
// static constexpr char NUM_TEMP_SENSORS[] = "num_temp_sensors";
// static constexpr char NUM_LINES[] = "num_lines";
static constexpr char PANEL_TEMPERATURES[] = "panel_temperatures";
static constexpr char PANEL_VOLTAGES[] = "panel_voltages";
static constexpr char PANEL_CURRENTS[] = "panel_currents";
static constexpr char NUM_PANLES[] = "num_panels";

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingMsg::PowerReadingType_t power_reading_type;
  uint8_t status;
  double voltage_v;
  double current_a;
  double mpp_position;
  // uint8_t num_temp_sensors;
  // uint8_t num_lines;
  uint8_t num_panels;
  double *panel_temperatures;
  double *panel_voltages;
  double *panel_currents;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PowerSolarReadingMsg
