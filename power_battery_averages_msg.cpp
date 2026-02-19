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
  check_and_encode_key(err, encode_key_value_uint8(
                                &map_encoder, PowerBatteryAveragesMsg::NUM_CELLS, d.num_cells));

  if (d.num_cells > 0) {
    PowerBatteryAveragesMsg::array_encoder_decoder_t
        array_list[PowerBatteryAveragesMsg::NUM_ARRAYS] = {
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_AVG, &d.cell_voltage_v_avg},
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MAX, &d.cell_voltage_v_max},
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MIN, &d.cell_voltage_v_min},
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_STDEV, &d.cell_voltage_v_stdev},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_AVG, &d.cell_temperature_c_avg},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MAX, &d.cell_temperature_c_max},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MIN, &d.cell_temperature_c_min},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_STDEV, &d.cell_temperature_c_stdev},
        };

    for (size_t i = 0; i < PowerBatteryAveragesMsg::NUM_ARRAYS; i++) {
      err = cbor_encode_text_stringz(&map_encoder, array_list[i].array_name);
      if (err != CborNoError) {
        bm_debug("cbor_encode_text_stringz failed for %s key: %d\n", array_list[i].array_name,
                 err);
        if (err != CborErrorOutOfMemory) {
          return err;
        }
      }

      CborEncoder arrayEncoder;
      err = cbor_encoder_create_array(&map_encoder, &arrayEncoder, d.num_cells);
      if (err != CborNoError) {
        bm_debug("cbor_encoder_create_array failed for %s array: %d\n",
                 array_list[i].array_name, err);
        if (err != CborErrorOutOfMemory) {
          return err;
        }
      }

      for (uint8_t j = 0; j < d.num_cells; j++) {
        err = cbor_encode_double(&arrayEncoder, (*array_list[i].array_pointer)[j]);
        if (err != CborNoError) {
          bm_debug("cbor_encode_double failed for %s value: %d\n", array_list[i].array_name,
                   err);
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
        bm_debug("cbor_encoder_close_container failed for %s array: %d\n",
                 array_list[i].array_name, err);
        if (err != CborErrorOutOfMemory) {
          return err;
        }
      }
    }
  }

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

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
  check_and_decode_key(
      err, decode_key_value_uint8(&d.num_cells, &value, PowerBatteryAveragesMsg::NUM_CELLS));

  // decode the arrays
  if (d.num_cells > 0) {
    PowerBatteryAveragesMsg::array_encoder_decoder_t
        array_list[PowerBatteryAveragesMsg::NUM_ARRAYS] = {
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_AVG, &d.cell_voltage_v_avg},
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MAX, &d.cell_voltage_v_max},
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MIN, &d.cell_voltage_v_min},
            {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_STDEV, &d.cell_voltage_v_stdev},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_AVG, &d.cell_temperature_c_avg},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MAX, &d.cell_temperature_c_max},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MIN, &d.cell_temperature_c_min},
            {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_STDEV, &d.cell_temperature_c_stdev},
        };

    for (size_t i = 0; i < PowerBatteryAveragesMsg::NUM_ARRAYS; i++) {
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
        bm_debug("expected array but got something else\n");
        return err;
      }

      if (*array_list[i].array_pointer == NULL) {
        CborValue array;
        err = cbor_value_enter_container(&value, &array);
        if (err != CborNoError) {
          bm_debug("cbor_value_enter_container failed for %s array: %d\n",
                   array_list[i].array_name, err);
          return err;
        }
#ifndef CI_TEST
        *array_list[i].array_pointer = (double *)bm_malloc(sizeof(double) * d.num_cells);
#else  // CI_TEST
        *array_list[i].array_pointer = (double *)malloc(sizeof(double) * d.num_cells);
#endif // CI_TEST

        if (*array_list[i].array_pointer == NULL) {
          return CborErrorOutOfMemory;
        }

        for (uint8_t j = 0; j < d.num_cells; j++) {
          err = cbor_value_get_double(&array, &(*array_list[i].array_pointer)[j]);
          if (err != CborNoError) {
            break;
          }
          err = cbor_value_advance(&array);
          if (err != CborNoError) {
            bm_debug("Failed to advance %s array\n", array_list[i].array_name);
            break;
          }
        }
        err = cbor_value_leave_container(&value, &array);
        if (err != CborNoError) {
          bm_debug("cbor_value_leave_container failed for %s array: %d\n",
                   array_list[i].array_name, err);
          return err;
        }
      } else {
        err = cbor_value_advance(&value);
        if (err != CborNoError) {
          return err;
        }
      }
    }
  }

  return err;
}
