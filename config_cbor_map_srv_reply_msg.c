#include "config_cbor_map_srv_reply_msg.h"
#include "bm_config.h"
#include "bm_os.h"

CborError config_cbor_map_reply_encode(ConfigCborMapReplyData *d,
                                       uint8_t *cbor_buffer, size_t size,
                                       size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    err = cbor_encoder_create_map(&encoder, &map_encoder,
                                  CONFIG_CBOR_MAP_REPLY_NUM_FIELDS);
    if (err != CborNoError) {
      bm_debug("cbor_encoder_create_map failed: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // node_id
    err = cbor_encode_text_stringz(&map_encoder, "node_id");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for node_id key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d->node_id);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for node_id value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // partition_id
    err = cbor_encode_text_stringz(&map_encoder, "partition_id");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for partition_id key: %d\n",
               err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d->partition_id);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for partition_id value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // success
    err = cbor_encode_text_stringz(&map_encoder, "success");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for success key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d->success);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for success value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // cbor_encoded_map_len
    err = cbor_encode_text_stringz(&map_encoder, "cbor_encoded_map_len");
    if (err != CborNoError) {
      bm_debug(
          "cbor_encode_text_stringz failed for cbor_encoded_map_len key: %d\n",
          err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d->cbor_encoded_map_len);
    if (err != CborNoError) {
      bm_debug("cbor_encode_uint failed for cbor_encoded_map_len value: %d\n",
               err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // cbor_data
    err = cbor_encode_text_stringz(&map_encoder, "cbor_data");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for cbor_data key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_byte_string(&map_encoder, d->cbor_data,
                                  d->cbor_encoded_map_len);
    if (err != CborNoError) {
      bm_debug("cbor_encode_byte_string failed for partition_id value: %d\n",
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

// allocates memory for d.cbor_data, caller must free.
CborError config_cbor_map_reply_decode(ConfigCborMapReplyData *d,
                                       const uint8_t *cbor_buffer,
                                       size_t size) {
  CborParser parser;
  CborValue map;
  CborError err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);
  uint64_t tmp_uint64;
  d->cbor_data = NULL;

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
    if (num_fields != CONFIG_CBOR_MAP_REPLY_NUM_FIELDS) {
      err = CborErrorUnknownLength;
      bm_debug("expected %zu fields but got %zu\n",
               CONFIG_CBOR_MAP_REPLY_NUM_FIELDS, num_fields);
      break;
    }

    CborValue value;
    err = cbor_value_enter_container(&map, &value);
    if (err != CborNoError) {
      break;
    }

    // node_id
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    d->node_id = tmp_uint64;
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // partition_id
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    d->partition_id = (uint32_t)tmp_uint64;
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // success
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    d->success = (bool)tmp_uint64;
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // cbor_encoded_map_len
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &tmp_uint64);
    d->cbor_encoded_map_len = (uint32_t)tmp_uint64;
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // cbor_data
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      bm_debug("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    if (d->cbor_encoded_map_len && d->success) {
      size_t buflen = d->cbor_encoded_map_len;
      uint8_t *buf = (uint8_t *)bm_malloc(buflen);
      if (buf) {
        err = cbor_value_copy_byte_string(&value, buf, &buflen, NULL);
        d->cbor_data = buf;
        if (err != CborNoError) {
          break;
        }
        if (buflen != d->cbor_encoded_map_len) {
          err = CborErrorIllegalType;
          break;
        }
      } else {
        d->cbor_data = NULL;
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
    }
  } while (0);

  return err;
}
