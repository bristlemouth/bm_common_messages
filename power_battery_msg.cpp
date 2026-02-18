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
  err = cbor_encode_text_stringz(&map_encoder, PowerBatteryMsg::CELL_VOLTAGE_V);
  if (err != CborNoError) {
    bm_debug("cbor_encode_text_stringz failed for cell_voltage_v key: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  CborEncoder arrayEncoder;
  err = cbor_encoder_create_array(&map_encoder, &arrayEncoder, d.num_cells);
  if (err != CborNoError) {
    bm_debug("cbor_encoder_create_array failed for cell_voltage_v array: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  for (uint8_t i = 0; i < d.num_cells; i++) {
    err = cbor_encode_double(&arrayEncoder, d.cell_voltage_v[i]);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for cell_voltage_v value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
  }
  if (err != CborNoError) {
    return err;
  }

  err = cbor_encoder_close_container(&map_encoder, &arrayEncoder);
  if (err != CborNoError) {
    bm_debug("cbor_encoder_close_container failed for cell_voltage_v array: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  err = cbor_encode_text_stringz(&map_encoder, PowerBatteryMsg::CELL_TEMPERATURE_C);
  if (err != CborNoError) {
    bm_debug("cbor_encode_text_stringz failed for cell_temperature_c key: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  CborEncoder arrayEncoder_temps;
  err = cbor_encoder_create_array(&map_encoder, &arrayEncoder_temps, d.num_cells);
  if (err != CborNoError) {
    bm_debug("cbor_encoder_create_array failed for cell_temperature_c array: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  for (uint8_t i = 0; i < d.num_cells; i++) {
    err = cbor_encode_double(&arrayEncoder_temps, d.cell_temperature_c[i]);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for cell_temperature_c value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
  }
  if (err != CborNoError) {
    return err;
  }

  err = cbor_encoder_close_container(&map_encoder, &arrayEncoder_temps);
  if (err != CborNoError) {
    bm_debug("cbor_encoder_close_container failed for cell_temperature_c array: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

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

  // decode the arrays
  // todo - handle what happens when the arrays are empty / not desired by the decoder??
  if (!cbor_value_is_text_string(&value)) {
    err = CborErrorIllegalType;
    bm_debug("expected string key but got something else\n");
    return err;
  }
  err = cbor_value_advance(&value);
  if (err != CborNoError) {
    return err;
  }
  if (!cbor_value_is_array(&value)) {
    err = CborErrorIllegalType;
    bm_debug("expected array key but got something else\n");
    return err;
  }

  if (d.cell_voltage_v == NULL && d.num_cells > 0) {
    CborValue array;
    err = cbor_value_enter_container(&value, &array);
    if (err != CborNoError) {
      bm_debug("cbor_value_enter_container failed for cell_voltage_v array: %d\n", err);
      return err;
    }
#ifndef CI_TEST
    d.cell_voltage_v = (double *)bm_malloc(sizeof(double) * d.num_cells);
#else  // CI_TEST
    d.cell_voltage_v = (double *)malloc(sizeof(double) * d.num_cells);
#endif // CI_TEST

    if (d.cell_voltage_v == NULL) {
      return CborErrorOutOfMemory;
    }

    for (uint8_t i = 0; i < d.num_cells; i++) {
      err = cbor_value_get_double(&array, &d.cell_voltage_v[i]);
      if (err != CborNoError) {
        break;
      }
      err = cbor_value_advance(&array);
      if (err != CborNoError) {
        bm_debug("Failed to advance cell_voltage_v array\n");
        break;
      }
    }
    err = cbor_value_leave_container(&value, &array);
    if (err != CborNoError) {
      bm_debug("cbor value_leave_container failed for cell_voltage_v array: %d\n", err);
      return err;
    }
  } else {
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      return err;
    }
  }

  if (!cbor_value_is_text_string(&value)) {
    err = CborErrorIllegalType;
    bm_debug("expected string key but got something else\n");
    return err;
  }
  err = cbor_value_advance(&value);
  if (err != CborNoError) {
    return err;
  }
  if (!cbor_value_is_array(&value)) {
    err = CborErrorIllegalType;
    bm_debug("expected array key but got something else\n");
    return err;
  }

  if (d.cell_temperature_c == NULL && d.num_cells > 0) {
    CborValue array_temps;
    err = cbor_value_enter_container(&value, &array_temps);
    if (err != CborNoError) {
      bm_debug("cbor_value_enter_container failed for cell_temperature_c array: %d\n", err);
      return err;
    }
#ifndef CI_TEST
    d.cell_temperature_c = (double *)bm_malloc(sizeof(double) * d.num_cells);
#else  // CI_TEST
    d.cell_temperature_c = (double *)malloc(sizeof(double) * d.num_cells);
#endif // CI_TEST

    if (d.cell_temperature_c == NULL) {
      return CborErrorOutOfMemory;
    }

    for (uint8_t i = 0; i < d.num_cells; i++) {
      err = cbor_value_get_double(&array_temps, &d.cell_temperature_c[i]);
      if (err != CborNoError) {
        break;
      }
      err = cbor_value_advance(&array_temps);
      if (err != CborNoError) {
        bm_debug("Failed to advance cell_temperature_c array\n");
        break;
      }
    }
    err = cbor_value_leave_container(&value, &array_temps);
    if (err != CborNoError) {
      bm_debug("cbor value_leave_container failed for cell_temperature_c array: %d\n", err);
      return err;
    }
  } else {
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      return err;
    }
  }

  return err;
}
