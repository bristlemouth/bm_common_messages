#pragma once
#include "cbor.h"
#include "power_reading_msg.h"
#include "sensor_header_msg.h"

namespace PowerBatteryAveragesMsg {

constexpr uint32_t VERSION = 1;
constexpr size_t NUM_FIELDS = 9 + SensorHeaderMsg::NUM_FIELDS;
constexpr size_t NUM_ARRAYS = 8;
static constexpr char NUM_CELLS[] = "num_cells";
static constexpr char CELL_VOLTAGE_V_AVG[] = "cell_voltage_v_avg";
static constexpr char CELL_VOLTAGE_V_MAX[] = "cell_voltage_v_max";
static constexpr char CELL_VOLTAGE_V_MIN[] = "cell_voltage_v_min";
static constexpr char CELL_VOLTAGE_V_STDEV[] = "cell_voltage_v_stdev";
static constexpr char CELL_TEMPERATURE_C_AVG[] = "cell_temperature_c_avg";
static constexpr char CELL_TEMPERATURE_C_MAX[] = "cell_temperature_c_max";
static constexpr char CELL_TEMPERATURE_C_MIN[] = "cell_temperature_c_min";
static constexpr char CELL_TEMPERATURE_C_STDEV[] = "cell_temperature_c_stdev";

typedef struct array_encoder_decoder {
  const char* array_name;
  double *array_pointer;
} array_encoder_decoder_t;

struct Data {
  SensorHeaderMsg::Data header;
  PowerReadingMsg::PowerReadingType_t power_reading_type;
  uint8_t status;
  uint8_t num_cells;
  double *cell_voltage_v_avg;
  double *cell_voltage_v_max;
  double *cell_voltage_v_min;
  double *cell_voltage_v_stdev;
  double *cell_temperature_c_avg;
  double *cell_temperature_c_max;
  double *cell_temperature_c_min;
  double *cell_temperature_c_stdev;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace PowerBatteryAveragesMsg
