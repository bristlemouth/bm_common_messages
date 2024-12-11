#include "bm_rbr_pressure_difference_signal_msg.h"
#include "bm_config.h"
#include <math.h>

namespace BmRbrPressureDifferenceSignalMsg {

/*!
 * \brief Encode the BmRbrPressureDifferenceSignalMsg::Data structure into a
 * CBOR buffer.
 *
 * \param[in] d The BmRbrPressureDifferenceSignalMsg::Data structure to encode.
 * \param[out] cbor_buffer The buffer to encode the data into.
 * \param[in] size The size of the buffer.
 * \param[out] encoded_len The length of the encoded data.
 *
 * \return CborError
 */
CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder, array_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    if (!cbor_buffer) {
      err = CborErrorOutOfMemory;
      break;
    }

    if (!d.difference_signal) {
      err = CborErrorOutOfMemory;
      break;
    }

    if (!d.num_samples) {
      err = CborErrorImproperValue;
      break;
    }

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

    // sequence_num
    err = cbor_encode_text_stringz(&map_encoder, "sequence_num");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for sequence_num key: %d\n",
               err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.sequence_num);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for sequence_num value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // total_samples
    err = cbor_encode_text_stringz(&map_encoder, "total_samples");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for total_samples key: %d\n",
               err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.total_samples);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for total_samples value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // num_samples
    err = cbor_encode_text_stringz(&map_encoder, "num_samples");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for num_samples key: %d\n",
               err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.num_samples);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for num_samples value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // residual_0
    err = cbor_encode_text_stringz(&map_encoder, "residual_0");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for residual_0 key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.residual_0);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for residual_0 value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // residual_1
    err = cbor_encode_text_stringz(&map_encoder, "residual_1");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for residual_1 key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_double(&map_encoder, d.residual_1);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for residual_1 value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // difference_signal
    err = cbor_encode_text_stringz(&map_encoder, "difference_signal");
    if (err != CborNoError) {
      bm_debug(
          "cbor_encode_text_stringz failed for difference_signal key: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err =
        cbor_encoder_create_array(&map_encoder, &array_encoder, d.num_samples);
    if (err != CborNoError) {
      bm_debug(
          "cbor_encoder_create_array failed for difference_signal value: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    for (size_t i = 0; i < d.num_samples; i++) {
      err = cbor_encode_double(&array_encoder, d.difference_signal[i]);
      if (err != CborNoError) {
        bm_debug("cbor_encode_double failed for difference_signal value: %d\n",
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
      bm_debug("cbor_encoder_close_container failed: %d\n", err);
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

/*!
 * \brief Decode a CBOR buffer into the BmRbrPressureDifferenceSignalMsg::Data
 * structure.
 *
 * \param[in/out] d The BmRbrPressureDifferenceSignalMsg::Data structure to
 * decode into. d.num_samples must be set to the maximum number of samples that
 * can be put into the buffer d.difference_signal must be a valid pointer to an
 * array of doubles of size d.num_samples
 * \param[in] cbor_buffer The buffer to
 * decode.
 * \param[in] size The size of the buffer.
 *
 * \return CborError
 */
CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map;
  CborError err = CborNoError;
  do {
    if (!cbor_buffer) {
      err = CborErrorOutOfMemory;
      break;
    }
    if (!d.difference_signal) {
      err = CborErrorOutOfMemory;
      break;
    }
    if (err != CborNoError) {
      break;
    }
    err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);
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

    // sequence_num
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    uint64_t sequence_num;
    err = cbor_value_get_uint64(&value, &sequence_num);
    if (err != CborNoError) {
      break;
    }
    d.sequence_num = sequence_num;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // total_samples
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    uint64_t total_samples;
    err = cbor_value_get_uint64(&value, &total_samples);
    if (err != CborNoError) {
      break;
    }
    d.total_samples = total_samples;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // num_samples
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
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

    // residual_0
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.residual_0);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // residual_1
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_double(&value, &d.residual_1);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // difference_signal
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    if (!cbor_value_is_array(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected array key but got something else\n");
      break;
    }
    size_t array_num_samples;
    err = cbor_value_get_array_length(&value, &array_num_samples);
    if (err != CborNoError) {
      break;
    }
    if (array_num_samples != d.num_samples) {
      err = CborErrorUnknownLength;
      bm_debug("expected %zu samples but got %zu\n", d.num_samples,
               array_num_samples);
      break;
    }

    CborValue array_elem;
    if (cbor_value_enter_container(&value, &array_elem) != CborNoError) {
      bm_debug("Failed to enter the array\n");
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
        bm_debug("Failed to advance the array\n");
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
