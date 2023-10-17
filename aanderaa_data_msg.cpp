#include "aanderaa_data_msg.h"
#include "FreeRTOS.h"

CborError AanderaaDataMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                  size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    err = cbor_encoder_create_map(&encoder, &map_encoder, NUM_FIELDS);
    if (err != CborNoError) {
      printf("cbor_encoder_create_map failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // abs_speed_mean_cm_s
    err = cbor_encode_text_stringz(&map_encoder, "abs_speed_mean_cm_s");
    if (err != CborNoError) {
      printf(
          "cbor_encode_text_stringz failed for abs_speed_mean_cm_s key: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.abs_speed_mean_cm_s);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for abs_speed_mean_cm_s value: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // abs_speed_stddev_cm_s
    err = cbor_encode_text_stringz(&map_encoder, "abs_speed_stddev_cm_s");
    if (err != CborNoError) {
      printf(
          "cbor_encode_text_stringz failed for abs_speed_stddev_cm_s key: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.abs_speed_stddev_cm_s);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for abs_speed_stddev_cm_s value: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // direction_circ_mean_rad
    err = cbor_encode_text_stringz(&map_encoder, "direction_circ_mean_rad");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for direction_circ_mean_rad key: "
             "%d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.direction_circ_mean_rad);
    if (err != CborNoError) {
      printf(
          "cbor_encode_double failed for direction_circ_mean_rad value: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // direction_circ_stddev_rad
    err = cbor_encode_text_stringz(&map_encoder, "direction_circ_stddev_rad");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for direction_circ_stddev_rad "
             "key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.direction_circ_stddev_rad);
    if (err != CborNoError) {
      printf(
          "cbor_encode_double failed for direction_circ_stddev_rad value: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // temperature_mean_degC
    err = cbor_encode_text_stringz(&map_encoder, "temperature_mean_degC");
    if (err != CborNoError) {
      printf(
          "cbor_encode_text_stringz failed for temperature_mean_degC key: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.temperature_mean_degC);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for temperature_mean_degC value: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    if (err != CborNoError && err != CborErrorOutOfMemory) {
      break;
    }

    err = cbor_encoder_close_container(&encoder, &map_encoder);
    if (err == CborNoError) {
      *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
    } else {
      printf("cbor_encoder_close_container failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
      size_t extra_bytes_needed = cbor_encoder_get_extra_bytes_needed(&encoder);
      printf("extra_bytes_needed: %zu\n", extra_bytes_needed);
    }
  } while (0);

  return err;
}

CborError AanderaaDataMsg::decode(Data &d, const uint8_t *cbor_buffer,
                                  size_t size) {
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
    if (num_fields != AanderaaDataMsg::NUM_FIELDS) {
      err = CborErrorUnknownLength;
      printf("expected %zu fields but got %zu\n", AanderaaDataMsg::NUM_FIELDS,
             num_fields);
      break;
    }

    CborValue value;
    err = cbor_value_enter_container(&map, &value);
    if (err != CborNoError) {
      break;
    }

    // abs_speed_mean_cm_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.abs_speed_mean_cm_s);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // abs_speed_stddev_cm_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.abs_speed_stddev_cm_s);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // direction_circ_mean_rad
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.direction_circ_mean_rad);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // direction_circ_stddev_rad
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.direction_circ_stddev_rad);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // temperature_mean_degC
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.temperature_mean_degC);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    if (err == CborNoError) {
      err = cbor_value_leave_container(&map, &value);
      if (err != CborNoError) {
        break;
      }
      if (!cbor_value_at_end(&map)) {
        err = CborErrorGarbageAtEnd;
        break;
      }
    }
  } while (0);

  return err;
}
