#include "power_battery_averages_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"
#ifndef CI_TEST
#include "bm_os.h"
#endif

CborError PowerBatteryAveragesMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len) {
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
  check_and_encode_key(err, encode_key_value_uint8(
                                &map_encoder, PowerBatteryAveragesMsg::NUM_CELLS, d.num_cells));

  // TODO - encode the arrays!
  if (d.num_cells > 0) {

    PowerBatteryAveragesMsg::array_encoder_decoder_t array_list[PowerBatteryAveragesMsg::NUM_ARRAYS] = {
      {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_AVG, d.cell_voltage_v_avg},
      {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MAX, d.cell_voltage_v_max},
      {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_MIN, d.cell_voltage_v_min},
      {PowerBatteryAveragesMsg::CELL_VOLTAGE_V_STDEV, d.cell_voltage_v_stdev},
      {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_AVG, d.cell_temperature_c_avg},
      {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MAX, d.cell_temperature_c_max},
      {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_MIN, d.cell_temperature_c_min},
      {PowerBatteryAveragesMsg::CELL_TEMPERATURE_C_STDEV, d.cell_temperature_c_stdev},
    };


    for (size_t i = 0; i < PowerBatteryAveragesMsg::NUM_ARRAYS; i++) {
      err = cbor_encode_text_stringz(&map_encoder, array_list[i].array_name);
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

      for (uint8_t j = 0; j < d.num_cells; j++) {
        err = cbor_encode_double(&arrayEncoder, array_list[i].array_pointer[j]);
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
  (void) d;
  (void) cbor_buffer;
  (void) size;

  CborError err = CborNoError;

  // TODO - implement the decoder!

  return err;
}
