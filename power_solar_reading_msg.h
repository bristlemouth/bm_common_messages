#pragma once
#include "cbor.h"
#include "power_reading_msg.h"
#include "sensor_header_msg.h"

namespace PowerSolarReadingMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 8 + SensorHeaderMsg::NUM_FIELDS;
static constexpr char MPP_POSITION[] = "mpp_position";
static constexpr char PANEL_TEMPERATURES[] = "panel_temperatures";
static constexpr char PANEL_VOLTAGES[] = "panel_voltages";
static constexpr char PANEL_CURRENTS[] = "panel_currents";

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingMsg::PowerReadingType_t power_reading_type;
  uint8_t status;
  double voltage_v;
  double current_a;
  double mpp_position;
  uint8_t num_temp_sensors;
  double *panel_temperatures;
  uint8_t num_lines;
  double *panel_voltages;
  double *panel_currents;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

void free(Data &d);

} // namespace PowerSolarReadingMsg
