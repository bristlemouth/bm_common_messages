#pragma once
#include "cbor.h"
#include "power_reading_msg.h"
#include "sensor_header_msg.h"

namespace PowerSolarAveragesMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 16 + SensorHeaderMsg::NUM_FIELDS;
static constexpr char NUM_SAMPLES[] = "num_samples";
static constexpr char AVERAGING_WINDOW_LENGTH_S[] = "averaging_window_length_s";
static constexpr char PANEL_TEMPERATURES_AVERAGE[] = "panel_temperatures_average";
static constexpr char PANEL_TEMPERATURES_MAX[] = "panel_temperatures_max";
static constexpr char PANEL_TEMPERATURES_MIN[] = "panel_temperatures_min";
static constexpr char PANEL_TEMPERATURES_STDEV[] = "panel_temperatures_stdev";
static constexpr char PANEL_VOLTAGES_AVERAGE[] = "panel_voltages_average";
static constexpr char PANEL_VOLTAGES_MAX[] = "panel_voltages_max";
static constexpr char PANEL_VOLTAGES_MIN[] = "panel_voltages_min";
static constexpr char PANEL_VOLTAGES_STDEV[] = "panel_voltages_stdev";
static constexpr char PANEL_CURRENTS_AVERAGE[] = "panel_currents_average";
static constexpr char PANEL_CURRENTS_MAX[] = "panel_currents_max";
static constexpr char PANEL_CURRENTS_MIN[] = "panel_currents_min";
static constexpr char PANEL_CURRENTS_STDEV[] = "panel_currents_stdev";

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingMsg::PowerReadingType_t power_reading_type;
  uint8_t status;
  uint32_t num_samples;
  double averaging_window_length_s;
  uint8_t num_temp_sensors; // Not sent in cbor
  uint8_t num_lines;  // Not sent in cbor
  double *panel_temperatures_average;
  double *panel_temperatures_max;
  double *panel_temperatures_min;
  double *panel_temperatures_stdev;
  double *panel_voltages_average;
  double *panel_voltages_max;
  double *panel_voltages_min;
  double *panel_voltages_stdev;
  double *panel_currents_average;
  double *panel_currents_max;
  double *panel_currents_min;
  double *panel_currents_stdev;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

void free(Data &d);

} // namespace PowerSolarAveragesMsg
