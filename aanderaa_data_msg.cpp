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

    // sensor_header_msg
    err = cbor_encode_text_stringz(&map_encoder, "header");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for version key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = SensorHeaderMsg::encode(d.header, cbor_buffer, size, encoded_len);
    if (err != CborNoError) {
      printf("SensorHeaderMsg::encode failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // abs_speed_cm_s
    err = cbor_encode_text_stringz(&map_encoder, "abs_speed_cm_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for abs_speed_cm_s key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.abs_speed_cm_s);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for abs_speed_cm_s value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // direction_deg_m
    err = cbor_encode_text_stringz(&map_encoder, "direction_deg_m");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for direction_deg_m key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.direction_deg_m);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for direction_deg_m value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // north_cm_s
    err = cbor_encode_text_stringz(&map_encoder, "north_cm_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for north_cm_s key: "
             "%d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.north_cm_s);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for north_cm_s value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // east_cm_s
    err = cbor_encode_text_stringz(&map_encoder, "east_cm_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for east_cm_s "
             "key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.east_cm_s);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for east_cm_s value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // heading_deg_m
    err = cbor_encode_text_stringz(&map_encoder, "heading_deg_m");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for heading_deg_m key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.heading_deg_m);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for heading_deg_m value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // tilt_x_deg
    err = cbor_encode_text_stringz(&map_encoder, "tilt_x_deg");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for tilt_x_deg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.tilt_x_deg);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for tilt_x_deg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // tilt_y_deg
    err = cbor_encode_text_stringz(&map_encoder, "tilt_y_deg");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for tilt_y_deg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.tilt_y_deg);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for tilt_y_deg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // standard_ping_std_cm_s
    err = cbor_encode_text_stringz(&map_encoder, "standard_ping_std_cm_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for standard_ping_std_cm_s key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.standard_ping_std_cm_s);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for standard_ping_std_cm_s value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // transducer_strength_db
    err = cbor_encode_text_stringz(&map_encoder, "transducer_strength_db");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for transducer_strength_db key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.transducer_strength_db);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for transducer_strength_db value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // ping_count
    err = cbor_encode_text_stringz(&map_encoder, "ping_count");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for ping_count key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.ping_count);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for ping_count value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // abs_tilt_deg
    err = cbor_encode_text_stringz(&map_encoder, "abs_tilt_deg");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for abs_tilt_deg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.abs_tilt_deg);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for abs_tilt_deg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // max_tilt_deg
    err = cbor_encode_text_stringz(&map_encoder, "max_tilt_deg");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for max_tilt_deg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.max_tilt_deg);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for max_tilt_deg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // std_tilt_deg
    err = cbor_encode_text_stringz(&map_encoder, "std_tilt_deg");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for std_tilt_deg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.std_tilt_deg);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for std_tilt_deg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // temperature_deg_c
    err = cbor_encode_text_stringz(&map_encoder, "temperature_deg_c");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for temperature_deg_c key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.temperature_deg_c);
    if (err != CborNoError) {
      printf("cbor_encode_double failed for temperature_deg_c value: %d\n",
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

    // header
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = SensorHeaderMsg::decode(d.header, cbor_buffer, size);
    if (err != CborNoError) {
      break;
    }

    // abs_speed_cm_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.abs_speed_cm_s);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // direction_deg_m
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.direction_deg_m);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // north_cm_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.north_cm_s);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // east_cm_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.east_cm_s);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // heading_deg_m
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.heading_deg_m);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // tilt_x_deg
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.tilt_x_deg);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // tilt_y_deg
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.tilt_y_deg);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // standard_ping_std_cm_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.standard_ping_std_cm_s);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // transducer_strength_db
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.transducer_strength_db);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // ping_count
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.ping_count);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // abs_tilt_deg
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.abs_tilt_deg);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // max_tilt_deg
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.max_tilt_deg);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // std_tilt_deg
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.std_tilt_deg);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // temperature_deg_c
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.temperature_deg_c);
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
