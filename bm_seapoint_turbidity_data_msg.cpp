#include "bm_seapoint_turbidity_data_msg.h"
#include "bm_config.h"
#include <math.h>

namespace BmSeapointTurbidityDataMsg {

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    err = cbor_encoder_create_map(&encoder, &map_encoder, NUM_FIELDS);
    if (err != CborNoError) {
      bm_debug("cbor_encoder_create_map failed: %d\n", err);
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

    // s_signal
    err = cbor_encode_text_stringz(&map_encoder, "s_signal");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for s_signal key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.s_signal);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for s_signal value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // r_signal
    err = cbor_encode_text_stringz(&map_encoder, "r_signal");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for r_signal key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.r_signal);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for r_signal value: %d\n", err);
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

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
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
    if (num_fields != NUM_FIELDS) {
      err = CborErrorUnknownLength;
      bm_debug("expected %zu fields but got %zu\n", NUM_FIELDS, num_fields);
      break;
    }

    CborValue value;
    err = cbor_value_enter_container(&map, &value);
    if (err != CborNoError) {
      break;
    }

    // header
    err = SensorHeaderMsg::decode(value, d.header);
    if (err != CborNoError) {
      break;
    }

    // s_signal
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.s_signal);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // r_signal
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.r_signal);
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

} // namespace BmSeapointTurbidityDataMsg
