#include "sensor_header_msg.h"
#include "FreeRTOS.h"

CborError SensorHeaderMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
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

    // version
    err = cbor_encode_text_stringz(&map_encoder, "version");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for version key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.version);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for version value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // reading_time_utc_s
    err = cbor_encode_text_stringz(&map_encoder, "reading_time_utc_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for reading_time_utc_s key: "
             "%d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.reading_time_utc_s);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for reading_time_utc_s value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // reading_uptime_millis
    err = cbor_encode_text_stringz(&map_encoder, "reading_uptime_millis");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for reading_uptime_millis key: "
             "%d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.reading_uptime_millis);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for reading_uptime_millis value: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    //  sensor_reading_time_s
    err = cbor_encode_text_stringz(&map_encoder, "north_cm_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for north_cm_s key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.sensor_reading_time_s);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for sensor_reading_time_s value: %d\n",
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

CborError SensorHeaderMsg::decode(Data &d, const uint8_t *cbor_buffer,
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
    if (num_fields != SensorHeaderMsg::NUM_FIELDS) {
      err = CborErrorUnknownLength;
      printf("expected %zu fields but got %zu\n", SensorHeaderMsg::NUM_FIELDS,
             num_fields);
      break;
    }

    CborValue value;
    err = cbor_value_enter_container(&map, &value);
    if (err != CborNoError) {
      break;
    }

    // version
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    uint32_t tmp_uint64;
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.version = tmp_uint64;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // reading_time_utc_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.reading_time_utc_s = tmp_uint64;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // reading_uptime_millis
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.reading_uptime_millis = tmp_uint64;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // sensor_reading_time_s
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.sensor_reading_time_s = tmp_uint64;
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
