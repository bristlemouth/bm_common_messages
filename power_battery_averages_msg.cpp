#include "power_battery_averages_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"
#ifndef CI_TEST
#include "bm_os.h"
#endif

CborError PowerBatteryAveragesMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                          size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                               PowerBatteryAveragesMsg::NUM_FIELDS);

  // sensor_header_msg
  err = SensorHeaderMsg::encode(map_encoder, d.header);
  if (err != CborNoError) {
    bm_debug("SensorHeaderMsg::encode failed: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  check_and_encode_key(err,
                       encode_key_value_uint8(&map_encoder, PowerReadingMsg::POWER_READING_TYPE,
                                              d.power_reading_type));
  check_and_encode_key(err,
                       encode_key_value_uint8(&map_encoder, PowerReadingMsg::STATUS, d.status));
  check_and_encode_key(err, encode_key_value_uint32(&map_encoder,
                                                    PowerBatteryAveragesMsg::NUM_SAMPLES,
                                                    d.num_samples));
  check_and_encode_key(
      err,
      encode_key_value_double(&map_encoder, PowerBatteryAveragesMsg::AVERAGING_WINDOW_LENGTH_S,
                              d.averaging_window_length_s));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_VOLTAGE_V_AVG,
                                d.cell_voltage_v_avg, d.num_cell_voltages));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MAX,
                                d.cell_voltage_v_max, d.num_cell_voltages));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MIN,
                                d.cell_voltage_v_min, d.num_cell_voltages));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_VOLTAGE_V_STDEV,
                                d.cell_voltage_v_stdev, d.num_cell_voltages));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_AVG,
                                d.cell_temperature_c_avg, d.num_temp_sensors));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MAX,
                                d.cell_temperature_c_max, d.num_temp_sensors));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MIN,
                                d.cell_temperature_c_min, d.num_temp_sensors));
  check_and_encode_key(err, encode_key_value_double_array(
                                &map_encoder, PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_STDEV,
                                d.cell_temperature_c_stdev, d.num_temp_sensors));

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

/*!
 @brief Decodes a PowerBatteryAveragesMsg from a CBOR buffer

 @details This function decodes a CBOR-encoded PowerBatteryAveragesMsg and populates
 the provided Data structure. It decodes the sensor header, simple fields, and
 dynamically sized arrays for cell voltage and temperature statistics.

 **MEMORY ALLOCATION**: This function allocates memory for all array fields in the
 Data structure using bm_malloc().

 **REQUIREMENTS**: All array pointer fields in the Data structure MUST be initialized
 to NULL before calling this function. If an array pointer is already non-NULL, that
 array will be skipped during decoding.

 **CALLER RESPONSIBILITY**: The caller is responsible for freeing all allocated array
 memory when no longer needed using bm_free() or the provided PowerBatteryAveragesMsg::free.

 @param d Reference to Data structure to populate. Array pointers must be NULL.
 @param cbor_buffer Pointer to the CBOR-encoded message buffer
 @param size Size of the CBOR buffer in bytes

 @return CborError - CborNoError on success, or appropriate error code:
         - CborErrorIllegalType if unexpected CBOR types are encountered
         - CborErrorOutOfMemory if memory allocation fails
         - Other CBOR errors from underlying decode operations
 */
CborError PowerBatteryAveragesMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map, value;
  CborError err;

  err = decoder_message_enter(&map, &value, &parser, (uint8_t *)cbor_buffer, size,
                              PowerBatteryAveragesMsg::NUM_FIELDS);
  if (err != CborNoError) {
    return err;
  }

  // decode header
  err = SensorHeaderMsg::decode(value, d.header);
  if (err != CborNoError) {
    return err;
  }

  check_and_decode_key(err, decode_key_value_uint8((uint8_t *)&d.power_reading_type, &value,
                                                   PowerReadingMsg::POWER_READING_TYPE));
  check_and_decode_key(err, decode_key_value_uint8(&d.status, &value, PowerReadingMsg::STATUS));
  check_and_decode_key(err, decode_key_value_uint32(&d.num_samples, &value,
                                                    PowerBatteryAveragesMsg::NUM_SAMPLES));
  check_and_decode_key(
      err, decode_key_value_double(&d.averaging_window_length_s, &value,
                                   PowerBatteryAveragesMsg::AVERAGING_WINDOW_LENGTH_S));
  // decode the arrays
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_voltage_v_avg, &d.num_cell_voltages, &value,
                                         PowerBatteryAveragesMsg::CELL_VOLTAGE_V_AVG));
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_voltage_v_max, &d.num_cell_voltages, &value,
                                         PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MAX));
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_voltage_v_min, &d.num_cell_voltages, &value,
                                         PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MIN));
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_voltage_v_stdev, &d.num_cell_voltages, &value,
                                         PowerBatteryAveragesMsg::CELL_VOLTAGE_V_STDEV));
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_temperature_c_avg, &d.num_temp_sensors, &value,
                                         PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_AVG));
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_temperature_c_max, &d.num_temp_sensors, &value,
                                         PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MAX));
  check_and_decode_key(
      err, decode_key_value_double_array(&d.cell_temperature_c_min, &d.num_temp_sensors, &value,
                                         PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MIN));
  check_and_decode_key(err, decode_key_value_double_array(
                                &d.cell_temperature_c_stdev, &d.num_temp_sensors, &value,
                                PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_STDEV));

  if (check_acceptable_decode_errors(err)) {
    err = decoder_message_leave(&value, &map);
  }

  return err;
}

/*!
 @brief Helper function to safely free a double pointer and set it to NULL

 @details Checks if the pointer is non-NULL before freeing, then sets it to NULL
 to prevent double-free errors. Uses platform-appropriate free function.

 @param pointer Pointer to a double pointer to free
 */
static inline void freePointer(double **pointer) {
  if (*pointer) {
#ifndef CI_TEST
    bm_free(*pointer);
#else
    free(*pointer);
#endif
    *pointer = NULL;
  }
}

/*!
 @brief Frees all dynamically allocated memory in a PowerBatteryAveragesMsg Data structure

 @details This function safely deallocates all array fields in the Data structure that
 were allocated during decode operations. Each pointer is checked for NULL before
 freeing, and all pointers are set to NULL after deallocation to prevent double-free
 errors.

 **SAFE TO CALL MULTIPLE TIMES**: This function can be called multiple times on the
 same Data structure without causing crashes or undefined behavior.

 **SAFE WITH UNINITIALIZED DATA**: This function can be safely called on Data structures
 where array pointers are already NULL (e.g., freshly initialized or already freed).

 **MEMORY FREED**: The following array fields are deallocated:
 - cell_voltage_v_avg
 - cell_voltage_v_max
 - cell_voltage_v_min
 - cell_voltage_v_stdev
 - cell_temperature_c_avg
 - cell_temperature_c_max
 - cell_temperature_c_min
 - cell_temperature_c_stdev

 @param d Reference to Data structure containing arrays to free. After this call,
          all array pointers will be set to NULL.
 */
void PowerBatteryAveragesMsg::free(Data &d) {
  freePointer(&d.cell_voltage_v_avg);
  freePointer(&d.cell_voltage_v_max);
  freePointer(&d.cell_voltage_v_min);
  freePointer(&d.cell_voltage_v_stdev);
  freePointer(&d.cell_temperature_c_avg);
  freePointer(&d.cell_temperature_c_max);
  freePointer(&d.cell_temperature_c_min);
  freePointer(&d.cell_temperature_c_stdev);
  return;
}
