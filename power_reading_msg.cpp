#include "power_reading_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"

CborError PowerReadingMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                  size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    err = cbor_encoder_create_map(&encoder, &map_encoder, NUM_FIELDS);
    if (err != CborNoError) {
      bm_debug("cbor_encoer_create_map failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // sensor_header_msg
    err = SensorHeaderMsg::encode(map_encoder, d.header);
    if (err != CborNoError) {
      bm_debug("SensorHeaderMsg::encode failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    const CborEncoderTableEntry_t power_table_entries[] = {
        {PowerReadingMsg::POWER_READING_TYPE, UINT8, &d.power_reading_type},
        {PowerReadingMsg::VOLTAGE_V, DOUBLE, &d.voltage_v},
        {PowerReadingMsg::CURRENT_A, DOUBLE, &d.current_a},
        {PowerReadingMsg::STATUS, UINT8, &d.status}};

    err = encode_cbor_fields_from_table(&map_encoder, power_table_entries,
                                  sizeof(power_table_entries) / sizeof(power_table_entries[0]));

    if (err != CborNoError && err != CborErrorOutOfMemory) {
      break;
    }

    err = cbor_encoder_close_container(&encoder, &map_encoder);
    if (err == CborNoError) {
      *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
    } else {
      bm_debug("cbor_encoder_close_container failed: %d\n", err);

      if (err != CborErrorOutOfMemory) {
        break;
      }
      size_t extra_bytes_needed = cbor_encoder_get_extra_bytes_needed(&encoder);
      bm_debug("extra_bytes_needed: %zu\n", extra_bytes_needed);
    }
  } while (0);

  return err;
}

CborError PowerReadingMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map;
  CborError err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);

  do {
    if (err != CborNoError) {
      break;
    }

    err = cbor_value_validate_basic(&map);
    if (err != CborNoError) {
      break;
    }

    if (!cbor_value_is_map(&map)) {
      err = CborErrorIllegalType;
      break;
    }

    size_t num_fields;
    err = cbor_value_get_map_length(&map, &num_fields);
    if (err != CborNoError) {
      break;
    }

    if (num_fields != PowerReadingMsg::NUM_FIELDS) {
      bm_debug("expected %zu fields but got %zu\n", PowerReadingMsg::NUM_FIELDS, num_fields);
    }

    CborValue value;
    err = cbor_value_enter_container(&map, &value);
    if (err != CborNoError) {
      break;
    }

    // decode header
    err = SensorHeaderMsg::decode(value, d.header);
    if (err != CborNoError) {
      break;
    }

    // Use look up table to decode the known key-value pairs
    const CborDecoderTableEntry_t power_table_entries[] = {
        {PowerReadingMsg::POWER_READING_TYPE, UINT8, &d.power_reading_type},
        {PowerReadingMsg::VOLTAGE_V, DOUBLE, &d.voltage_v},
        {PowerReadingMsg::CURRENT_A, DOUBLE, &d.current_a},
        {PowerReadingMsg::STATUS, UINT8, &d.status},
    };

    err = decode_cbor_fields_from_table(&value, power_table_entries,
                                        sizeof(power_table_entries) /
                                            sizeof(power_table_entries[0]));

    // If the look up table decoding failed, then just go straight to the return
    if (err != CborNoError) {
      break;
    }

    err = cbor_value_leave_container(&map, &value);
    if (err != CborNoError) {
      break;
    }
    if (!cbor_value_at_end(&map)) {
      err = CborErrorTooManyItems;
      break;
    }
  } while (0);

  return err;
}
