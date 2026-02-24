#include "power_battery_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"
#ifndef CI_TEST
#include "bm_os.h"
#endif

CborError PowerBatteryMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                  size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                               PowerBatteryMsg::NUM_FIELDS);

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
  check_and_encode_key(
      err, encode_key_value_double(&map_encoder, PowerBatteryMsg::CHARGE_AH, d.charge_ah));
  check_and_encode_key(
      err, encode_key_value_double(&map_encoder, PowerBatteryMsg::CAPACITY_AH, d.capacity_ah));
  check_and_encode_key(
      err, encode_key_value_double(&map_encoder, PowerBatteryMsg::PERCENTAGE, d.percentage));
  check_and_encode_key(
      err,
      encode_key_value_uint8(&map_encoder, PowerBatteryMsg::BATTERY_STATUS, d.battery_status));
  check_and_encode_key(
      err,
      encode_key_value_uint8(&map_encoder, PowerBatteryMsg::BATTERY_HEALTH, d.battery_health));
  check_and_encode_key(
      err, encode_key_value_uint8(&map_encoder, PowerBatteryMsg::NUM_CELLS, d.num_cells));
  check_and_encode_key(err, encode_key_value_double_array(&map_encoder,
                                                          PowerBatteryMsg::CELL_VOLTAGE_V,
                                                          d.cell_voltage_v, d.num_cells));
  check_and_encode_key(err, encode_key_value_double_array(&map_encoder,
                                                          PowerBatteryMsg::CELL_TEMPERATURE_C,
                                                          d.cell_temperature_c, d.num_cells));

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

CborError PowerBatteryMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map, value;
  CborError err;

  err = decoder_message_enter(&map, &value, &parser, (uint8_t *)cbor_buffer, size,
                              PowerBatteryMsg::NUM_FIELDS);
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
  check_and_decode_key(
      err, decode_key_value_double(&d.charge_ah, &value, PowerBatteryMsg::CHARGE_AH));
  check_and_decode_key(
      err, decode_key_value_double(&d.capacity_ah, &value, PowerBatteryMsg::CAPACITY_AH));
  check_and_decode_key(
      err, decode_key_value_double(&d.percentage, &value, PowerBatteryMsg::PERCENTAGE));
  check_and_decode_key(err, decode_key_value_uint8((uint8_t *)&d.battery_status, &value,
                                                   PowerBatteryMsg::BATTERY_STATUS));
  check_and_decode_key(err, decode_key_value_uint8((uint8_t *)&d.battery_health, &value,
                                                   PowerBatteryMsg::BATTERY_HEALTH));
  check_and_decode_key(
      err, decode_key_value_uint8(&d.num_cells, &value, PowerBatteryMsg::NUM_CELLS));

  check_and_decode_key(err,
                       decode_key_value_double_array(&d.cell_voltage_v, d.num_cells, &value,
                                                     PowerBatteryMsg::CELL_VOLTAGE_V));
  check_and_decode_key(err,
                       decode_key_value_double_array(&d.cell_temperature_c, d.num_cells, &value,
                                                     PowerBatteryMsg::CELL_TEMPERATURE_C));

  if (check_acceptable_decode_errors(err)) {
    err = decoder_message_leave(&value, &map);
  }

  return err;
}
