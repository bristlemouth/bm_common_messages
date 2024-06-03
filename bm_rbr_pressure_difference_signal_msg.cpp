#include "bm_rbr_pressure_difference_signal_msg.h"
#include <math.h>

namespace BmRbrPressureDifferenceSignalMsg {

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder, array_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    if (!d.difference_signal) {
      err = CborErrorImproperValue;
      break;
    }

    if (!d.num_samples) {
      err = CborErrorImproperValue;
      break;
    }

    err = cbor_encoder_create_map(&encoder, &map_encoder, NUM_FIELDS);
    if (err != CborNoError) {
      printf("cbor_encoder_create_map failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // sensor_header_msg
    err = SensorHeaderMsg::encode(map_encoder, d.header);
    if (err != CborNoError) {
      printf("SensorHeaderMsg::encode failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // num_samples
    err = cbor_encode_text_stringz(&map_encoder, "num_samples");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for num_samples key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.num_samples);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for num_samples value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // difference_signal
    err = cbor_encode_text_stringz(&map_encoder, "difference_signal");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for difference_signal key: %d\n",
             err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err =
        cbor_encoder_create_array(&map_encoder, &array_encoder, d.num_samples);
    if (err != CborNoError) {
      printf(
          "cbor_encoder_create_array failed for difference_signal value: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    for (size_t i = 0; i < d.num_samples; i++) {
      err = cbor_encode_double(&array_encoder, d.difference_signal[i]);
      if (err != CborNoError) {
        printf("cbor_encode_double failed for difference_signal value: %d\n",
               err);
        if (err != CborErrorOutOfMemory) {
          break;
        }
      }
    }

    if (err != CborNoError && err != CborErrorOutOfMemory) {
      break;
    }

    err = cbor_encoder_close_container(&map_encoder, &array_encoder);
    if (err != CborNoError) {
      printf("cbor_encoder_close_container failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
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

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map;
  CborError err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);

  do {
    if (!d.difference_signal) {
      err = CborErrorOutOfMemory;
      break;
    }
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
      printf("expected %zu fields but got %zu\n", NUM_FIELDS, num_fields);
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

    // num_samples
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    uint64_t num_samples;
    err = cbor_value_get_uint64(&value, &num_samples);
    if (err != CborNoError) {
      break;
    }
    if (d.num_samples < num_samples) {
      err = CborErrorOutOfMemory;
      break;
    }
    d.num_samples = num_samples;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // difference_signal
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    if (!cbor_value_is_array(&value)) {
      err = CborErrorIllegalType;
      printf("expected array key but got something else\n");
      break;
    }
    size_t array_num_samples;
    err = cbor_value_get_array_length(&value, &array_num_samples);
    if (err != CborNoError) {
      break;
    }
    if (array_num_samples != d.num_samples) {
      err = CborErrorUnknownLength;
      printf("expected %zu samples but got %zu\n", d.num_samples,
             array_num_samples);
      break;
    }

    CborValue array_elem;
    if (cbor_value_enter_container(&value, &array_elem) != CborNoError) {
      printf("Failed to enter the array\n");
      break;
    }

    for (size_t i = 0; i < d.num_samples; i++) {
      double sample;
      err = cbor_value_get_double(&array_elem, &sample);
      if (err != CborNoError) {
        break;
      }
      d.difference_signal[i] = sample;
      if (cbor_value_advance(&array_elem) != CborNoError) {
        printf("Failed to advance the array\n");
        break;
      }
    }

    if (err != CborNoError) {
      break;
    }

    err = cbor_value_leave_container(&value, &array_elem);
    if (err != CborNoError) {
      break;
    }

    if (cbor_value_at_end(&value)) {
      err = CborNoError;
    } else {
      err = CborErrorGarbageAtEnd;
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

} // namespace BmRbrPressureDifferenceSignalMsg