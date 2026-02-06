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

    // power_reading_type
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingMsg::POWER_READING_TYPE);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for power_reading_type key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_uint(&map_encoder, d.power_reading_type);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for power_reading_type value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // voltage_v
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingMsg::VOLTAGE_V);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for voltage_v key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.voltage_v);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for voltage_v value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // current_a
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingMsg::CURRENT_A);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for current_a key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.current_a);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for current_a value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // status
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingMsg::STATUS);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for status key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_uint(&map_encoder, d.status);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for status value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
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
  // used to indicate if the message has additional fields we cannot currently decode
  // bool unsupported_type = false;
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

    const CborDecoderTableEntry_t power_table_entries[] = {
      {PowerReadingMsg::POWER_READING_TYPE, UINT8, &d.power_reading_type},
      {PowerReadingMsg::VOLTAGE_V, DOUBLE, &d.voltage_v},
      {PowerReadingMsg::CURRENT_A, DOUBLE, &d.current_a},
      {PowerReadingMsg::STATUS, UINT8, &d.status},
    };

    err = decode_cbor_fields_from_table(&value, power_table_entries, sizeof(power_table_entries) / sizeof(power_table_entries[0]));

    // Loop through the known keys, ignoring any unknown keys, or missing keys
    // while (!cbor_value_at_end(&value)) {
    //   if (!cbor_value_is_text_string(&value)) {
    //     err = CborErrorIllegalType;
    //     bm_debug("expected string key but got something else\n");
    //     break;
    //   }

    //   size_t key_len;
    //   err = cbor_value_get_string_length(&value, &key_len);
    //   if (err != CborNoError) {
    //     break;
    //   }

    //   if (key_len > max_key_len - 1) {
    //     bm_debug("key too long: %zu\n", key_len);
    //     // Advance over the string key
    //     err = cbor_value_advance(&value);
    //     if (err != CborNoError) {
    //       break;
    //     }
    //     // Advance over the value
    //     err = cbor_value_advance(&value);
    //     if (err != CborNoError) {
    //       break;
    //     }
    //     // since the key was too long, loop to the next key value pair
    //     continue;
    //   }

    //   char key[max_key_len] = {0};
    //   err = cbor_value_copy_text_string(&value, key, &key_len, NULL);
    //   if (err != CborNoError) {
    //     break;
    //   }
    //   key[key_len] = '\0';

    //   // Advance over the string key
    //   err = cbor_value_advance(&value);
    //   if (err != CborNoError) {
    //     break;
    //   }

    //   // Get the known power reading msg types
    //   if (strcmp(PowerReadingMsg::POWER_READING_TYPE, key) == 0) {
    //     uint64_t tmp_power_reading_type;
    //     err = cbor_value_get_uint64(&value, &tmp_power_reading_type);
    //     if (err != CborNoError) {
    //       break;
    //     }
    //     d.power_reading_type = static_cast<PowerReadingType_t>(tmp_power_reading_type);
    //   } else if (strcmp(PowerReadingMsg::VOLTAGE_V, key) == 0) {
    //     err = cbor_value_get_double(&value, &d.voltage_v);
    //     if (err != CborNoError) {
    //       break;
    //     }
    //   } else if (strcmp(PowerReadingMsg::current_a, key) == 0) {
    //     err = cbor_value_get_double(&value, &d.current_a);
    //     if (err != CborNoError) {
    //       break;
    //     }
    //   } else if (strcmp(PowerReadingMsg::STATUS, key) == 0) {
    //     uint64_t tmp_status;
    //     err = cbor_value_get_uint64(&value, &tmp_status);
    //     if (err != CborNoError) {
    //       break;
    //     }
    //     d.status = static_cast<uint8_t>(tmp_status);
    //   } else {
    //     bm_debug("Additional field we cannot parse, ignoring\n");
    //     unsupported_type = true;
    //   }

    //   // Advance over the value
    //   err = cbor_value_advance(&value);
    //   if (err != CborNoError) {
    //     break;
    //   }
    // }

    // // If the loop above failed, then just go straight to the return
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

  // if (unsupported_type) {
  //   err = CborErrorUnsupportedType;
  // }

  return err;
}
