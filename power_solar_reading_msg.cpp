#include "power_solar_reading_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"
#ifndef CI_TEST
#include "bm_os.h"
#endif

CborError PowerSolarReadingMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                       size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                               PowerSolarReadingMsg::NUM_FIELDS);

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
  check_and_encode_key(
      err, encode_key_value_double(&map_encoder, PowerReadingMsg::VOLTAGE_V, d.voltage_v));
  check_and_encode_key(
      err, encode_key_value_double(&map_encoder, PowerReadingMsg::current_a, d.current_a));

  check_and_encode_key(err,
                       encode_key_value_double(&map_encoder, PowerSolarReadingMsg::MPP_POSITION,
                                               d.mpp_position));
  check_and_encode_key(
      err, encode_key_value_uint8(&map_encoder, PowerSolarReadingMsg::NUM_PANLES, d.num_panels));

  check_and_encode_key(
      err, encode_key_value_double_array(&map_encoder, PowerSolarReadingMsg::PANEL_TEMPERATURES,
                                         d.panel_temperatures, d.num_panels));
  check_and_encode_key(err, encode_key_value_double_array(&map_encoder,
                                                          PowerSolarReadingMsg::PANEL_VOLTAGES,
                                                          d.panel_voltages, d.num_panels));
  check_and_encode_key(err, encode_key_value_double_array(&map_encoder,
                                                          PowerSolarReadingMsg::PANEL_CURRENTS,
                                                          d.panel_currents, d.num_panels));

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

/*!
 @brief Decodes a PowerSolarReadingMsg from a CBOR buffer

 @details This function decodes a CBOR-encoded PowerSolarReadingMsg and populates
 the provided Data structure. It decodes the sensor header, power reading, and
 dynamically sized arrays for solar panel measurements.

 **MEMORY ALLOCATION**: This function allocates memory for all array fields in the
 Data structure using bm_malloc().

 **REQUIREMENTS**: All array pointer fields in the Data structure MUST be initialized
 to NULL before calling this function. If an array pointer is already non-NULL, that
 array will be skipped during decoding.

 **CALLER RESPONSIBILITY**: The caller is responsible for freeing all allocated array
 memory when no longer needed using bm_free().

 @param d Reference to Data structure to populate. Array pointers must be NULL.
 @param cbor_buffer Pointer to the CBOR-encoded message buffer
 @param size Size of the CBOR buffer in bytes

 @return CborError - CborNoError on success, or appropriate error code:
         - CborErrorIllegalType if unexpected CBOR types are encountered
         - CborErrorOutOfMemory if memory allocation fails
         - Other CBOR errors from underlying decode operations
 */
CborError PowerSolarReadingMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map, value;
  CborError err;

  err = decoder_message_enter(&map, &value, &parser, (uint8_t *)cbor_buffer, size,
                              PowerSolarReadingMsg::NUM_FIELDS);
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
  check_and_decode_key(
      err, decode_key_value_double(&d.voltage_v, &value, PowerReadingMsg::VOLTAGE_V));
  check_and_decode_key(
      err, decode_key_value_double(&d.current_a, &value, PowerReadingMsg::current_a));
  check_and_decode_key(err, decode_key_value_double(&d.mpp_position, &value,
                                                    PowerSolarReadingMsg::MPP_POSITION));
  check_and_decode_key(
      err, decode_key_value_uint8(&d.num_panels, &value, PowerSolarReadingMsg::NUM_PANLES));

  // decode the arrays
  check_and_decode_key(
      err, decode_key_value_double_array(&d.panel_temperatures, d.num_panels, &value,
                                         PowerSolarReadingMsg::PANEL_TEMPERATURES));
  check_and_decode_key(err,
                       decode_key_value_double_array(&d.panel_voltages, d.num_panels, &value,
                                                     PowerSolarReadingMsg::PANEL_VOLTAGES));
  check_and_decode_key(err,
                       decode_key_value_double_array(&d.panel_currents, d.num_panels, &value,
                                                     PowerSolarReadingMsg::PANEL_CURRENTS));

  if (check_acceptable_decode_errors(err)) {
    err = decoder_message_leave(&value, &map);
  }

  return err;
}
