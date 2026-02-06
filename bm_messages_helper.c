#include "bm_messages_helper.h"

#ifndef CI_TEST
#include "bm_os.h"
#else
#include <stdlib.h>
#endif

#if __has_include("bm_config.h")
#include "bm_config.h"
#else
#define bm_debug printf
#endif

#define check_and_run_get_api(e, f) check_and_decode_key(e, f)

CborError encoder_message_create(CborEncoder *encoder, CborEncoder *map_encoder,
                                 uint8_t *cbor_buffer, size_t size,
                                 size_t num_fields) {
  CborError err;

  cbor_encoder_init(encoder, cbor_buffer, size, 0);

  if ((err = cbor_encoder_create_map(encoder, map_encoder, num_fields)) !=
      CborNoError) {
    bm_debug("error: %s: cbor_encoder_create_map failed: %d\r\n", __func__,
             err);
  }

  return err;
}

CborError encode_key_value_float(CborEncoder *map_encoder, const char *name,
                                 const float value) {
  CborError err;
  if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
    bm_debug("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n",
             __func__, name, err);
    if (err != CborErrorOutOfMemory)
      return err;
  }

  if ((err = cbor_encode_float(map_encoder, value)) != CborNoError)
    bm_debug("error: %s(%s): cbor_encode_float() failed: %d\r\n", __func__,
             name, err);

  return err;
}

CborError encode_key_value_uint8(CborEncoder *map_encoder, const char *name,
                                 const uint8_t value) {
  CborError err;
  uint64_t tmp = (uint64_t)value;

  if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
    bm_debug("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n",
             __func__, name, err);
    if (err != CborErrorOutOfMemory)
      return err;
  }

  if ((err = cbor_encode_uint(map_encoder, tmp)) != CborNoError)
    bm_debug("error: %s(%s): cbor_encode_uint() failed: %d\r\n", __func__, name,
             err);
  return err;
}

CborError encode_key_value_uint32(CborEncoder *map_encoder, const char *name,
                                  const uint32_t value) {
  CborError err;
  uint64_t tmp = (uint64_t)value;

  if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
    bm_debug("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n",
             __func__, name, err);
    if (err != CborErrorOutOfMemory)
      return err;
  }

  if ((err = cbor_encode_uint(map_encoder, tmp)) != CborNoError)
    bm_debug("error: %s(%s): cbor_encode_uint() failed: %d\r\n", __func__, name,
             err);
  return err;
}

CborError encode_key_value_string(CborEncoder *map_encoder, const char *name,
                                  const char *value, const size_t len) {
  CborError err;
  if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
    bm_debug("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n",
             __func__, name, err);
    if (err != CborErrorOutOfMemory)
      return err;
  }

  if ((err = cbor_encode_text_string(map_encoder, value, len)) != CborNoError)
    bm_debug("error: %s(%s): cbor_encode_byte_string() failed: %d\r\n",
             __func__, name, err);
  return err;
}

CborError encode_key_value_bytes(CborEncoder *map_encoder, const char *name,
                                 const unsigned char *value, const size_t len) {
  CborError err;
  if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
    bm_debug("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n",
             __func__, name, err);
    if (err != CborErrorOutOfMemory)
      return err;
  }

  if ((err = cbor_encode_byte_string(map_encoder, value, len)) != CborNoError)
    bm_debug("error: %s(%s): cbor_encode_byte_string() failed: %d\r\n",
             __func__, name, err);
  return err;
}

CborError encoder_message_finish(CborEncoder *encoder,
                                 CborEncoder *map_encoder) {
  const CborError err = cbor_encoder_close_container(encoder, map_encoder);
  if (err != CborNoError)
    bm_debug("error: %s: cbor_encoder_close_container failed: %d\r\n", __func__,
             err);
  return err;
}

void encoder_message_check_memory(CborEncoder *encoder, CborError err) {
  /* we get here due to any error */
  if (CborErrorOutOfMemory == err) {
    bm_debug("error: %s: extra_bytes_needed: %zu\r\n", __func__,
             cbor_encoder_get_extra_bytes_needed(encoder));
  }
}

CborError decoder_message_enter(CborValue *map, CborValue *decode_value,
                                CborParser *parser, uint8_t *cbor_buffer,
                                size_t size, size_t num_fields) {
  CborError err;
  size_t container_num_fields;

  err = cbor_parser_init(cbor_buffer, size, 0, parser, map);

  check_and_run_get_api(err, cbor_value_validate_basic(map));

  if (check_acceptable_decode_errors(err) && !cbor_value_is_map(map)) {
    err = CborErrorIllegalType;
  }

  check_and_run_get_api(err,
                        cbor_value_get_map_length(map, &container_num_fields));

  if (check_acceptable_decode_errors(err) &&
      container_num_fields != num_fields) {
    err = CborErrorUnknownLength;
    bm_debug("error: %s: expected %zu fields but got %zu\r\n", __func__,
             num_fields, container_num_fields);
  }

  check_and_run_get_api(err, cbor_value_enter_container(map, decode_value));

  return err;
}

CborError decode_key_value_float(float *out, CborValue *value,
                                 const char *key_expected) {
  CborError err;

  if (!cbor_value_is_text_string(value)) {
    err = CborErrorIllegalType;
    bm_debug("error: %s(%s): expected string key but got something else\r\n",
             __func__, key_expected);
    return err;
  }

  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;
  if (!cbor_value_is_float(value))
    return CborErrorIllegalType;
  if ((err = cbor_value_get_float(value, out)) != CborNoError)
    return err;
  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;

  return err;
}

CborError decode_key_value_uint8(uint8_t *out, CborValue *value,
                                 const char *key_expected) {
  CborError err;
  uint64_t tmp = 0;

  if (!cbor_value_is_text_string(value)) {
    err = CborErrorIllegalType;
    bm_debug("error: %s(%s): expected string key but got something else\r\n",
             __func__, key_expected);
    return err;
  }

  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;
  if (!cbor_value_is_unsigned_integer(value))
    return CborErrorIllegalType;
  if ((err = cbor_value_get_uint64(value, &tmp)) != CborNoError)
    return err;
  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;

  *out = (uint8_t)tmp;

  return err;
}

CborError decode_key_value_uint32(uint32_t *out, CborValue *value,
                                  const char *key_expected) {
  CborError err;
  uint64_t tmp = 0;

  if (!cbor_value_is_text_string(value)) {
    err = CborErrorIllegalType;
    bm_debug("error: %s(%s): expected string key but got something else\r\n",
             __func__, key_expected);
    return err;
  }

  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;
  if (!cbor_value_is_unsigned_integer(value))
    return CborErrorIllegalType;
  if ((err = cbor_value_get_uint64(value, &tmp)) != CborNoError)
    return err;
  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;

  *out = (uint32_t)tmp;

  return err;
}

static CborError decode_key_value_string_bytes(void **out, size_t *len,
                                               CborValue *value,
                                               const char *key_expected) {
  CborError err;
  if (!cbor_value_is_text_string(value)) {
    err = CborErrorIllegalType;
    bm_debug("error: %s(%s): expected string key but got something else\r\n",
             __func__, key_expected);
    return err;
  }

  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;

  size_t len_without_zeroterm = 0;
  if ((err = cbor_value_calculate_string_length(
           value, &len_without_zeroterm)) != CborNoError)
    return err;

  size_t allocation_size = len_without_zeroterm;

  if (value->type == CborTextStringType) {
    // Length with zeroterm
    allocation_size++;
  }
#ifndef CI_TEST
  *out = (char *)bm_malloc(allocation_size);
#else
  *out = (char *)malloc(allocation_size);
#endif
  if (!(*out))
    return err;

  if (value->type == CborTextStringType) {
    err = cbor_value_copy_text_string(value, *(char **)out, &allocation_size,
                                      NULL);
  } else if (value->type == CborByteStringType) {
    err = cbor_value_copy_byte_string(value, *(uint8_t **)out, &allocation_size,
                                      NULL);
  } else {
    err = CborErrorIllegalType;
  }

  if (err != CborNoError) {
#ifndef CI_TEST
    bm_free(*out);
#else
    free(*out);
#endif
    return err;
  }

  *len = len_without_zeroterm;

  if ((err = cbor_value_advance(value)) != CborNoError)
    return err;

  return err;
}

CborError decode_key_value_string(char **out, size_t *len, CborValue *value,
                                  const char *key_expected) {
  CborError err =
      decode_key_value_string_bytes((void **)out, len, value, key_expected);

  if (err == CborNoError) {
    /* explicitly zero terminate the allocation */
    (*out)[*len] = '\0';
  }

  return err;
}

CborError decode_key_value_bytes(uint8_t **out, size_t *len, CborValue *value,
                                 const char *key_expected) {
  return decode_key_value_string_bytes((void **)out, len, value, key_expected);
}

CborError decoder_message_leave(CborValue *value, CborValue *map) {
  CborError err;

  err = cbor_value_leave_container(map, value);

  if (check_acceptable_decode_errors(err) && !cbor_value_at_end(map)) {
    err = CborErrorGarbageAtEnd;
  }

  return err;
}

CborError decode_cbor_fields_from_table(CborValue *value, const CborDecoderTableEntry_t *entries_table, size_t table_size) {
  CborError err = CborNoError;
  bool has_unknown_key = false;

  // Loop through the known keys, ignoring any unknown keys, or missing keys
    while (!cbor_value_at_end(value)) {
      if (!cbor_value_is_text_string(value)) {
        err = CborErrorIllegalType;
        bm_debug("expected string key but got something else\n");
        break;
      }

      size_t key_len;
      err = cbor_value_get_string_length(value, &key_len);
      if (err != CborNoError) {
        break;
      }

      if (key_len > max_key_len - 1) {
        bm_debug("key too long: %zu\n", key_len);
        // Advance over the string key
        err = cbor_value_advance(value);
        if (err != CborNoError) {
          break;
        }
        // Advance over the value
        err = cbor_value_advance(value);
        if (err != CborNoError) {
          break;
        }
        // since the key was too long, loop to the next key value pair
        continue;
      }

      char key[max_key_len] = {0};
      err = cbor_value_copy_text_string(value, key, &key_len, NULL);
      if (err != CborNoError) {
        break;
      }
      key[key_len] = '\0';

      // Advance over the string key
      err = cbor_value_advance(value);
      if (err != CborNoError) {
        break;
      }

      // Search the look up table for the matching string key
      size_t index;
      bool key_in_table = false;
      for (index = 0; index < table_size; index++) {
        if (strcmp(entries_table[index].key, key) == 0) {
          key_in_table = true;
          switch (entries_table[index].type) {
            case UINT8: {
              uint64_t temp_value = 0;
              err = cbor_value_get_uint64(value, &temp_value);
              *(uint8_t *)entries_table[index].value_desitination = (uint8_t)temp_value;
              break;
            }
            case UINT16: {
              uint64_t temp_value = 0;
              err = cbor_value_get_uint64(value, &temp_value);
              *(uint16_t *)entries_table[index].value_desitination = (uint16_t)temp_value;
              break;
            }
            case UINT32: {
              uint64_t temp_value = 0;
              err = cbor_value_get_uint64(value, &temp_value);
              *(uint32_t *)entries_table[index].value_desitination = (uint32_t)temp_value;
              break;
            }
            case UINT64: {
              err = cbor_value_get_uint64(value, (uint64_t *)entries_table[index].value_desitination);
              break;
            }
            case FLOAT: {
              err = cbor_value_get_float(value, (float *)entries_table[index].value_desitination);
              break;
            }
            case DOUBLE: {
              err = cbor_value_get_double(value, (double *)entries_table[index].value_desitination);
              break;
            }
            // TODO - this one needs more work
            // case STRING: {
            //   err = cbor_value_copy_text_string(&value, (char *)entries_table[index].value_desitination, &key_len, NULL);
            //   break;
            // }
            default: {
              bm_debug("Failed to decode value for key %s, err: %d", entries_table[index].key, err);
              err = CborErrorUnsupportedType;
              break;
            }
          }
          // We have found our key and decoded the value so exit the for loop
          break;
        }
      }

      if (!key_in_table) {
        bm_debug("Ignoring unknown key-value pair\n");
        has_unknown_key = true;
      }

      // Advance over the value to move to next key-value pair
      err = cbor_value_advance(value);
      if (err != CborNoError) {
        break;
      }
    }

    if (has_unknown_key && err == CborNoError) {
      err = CborErrorUnsupportedType;
    }

  return err;
}

CborError encode_cbor_fields_from_table(CborEncoder *map_encoder, const CborEncoderTableEntry_t *entries_table, size_t table_size) {
  CborError err = CborNoError;

  size_t index;
  for (index = 0; index < table_size; index++) {
    // Encode the key
    err = cbor_encode_text_stringz(map_encoder, entries_table[index].key);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for key: %s, err: %d\n", entries_table[index].key, err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // Encode the value based on the type
    switch (entries_table[index].type) {
      case UINT8: {
        err = cbor_encode_uint(map_encoder, *(const uint8_t *)entries_table[index].value_source);
        break;
      }
      case UINT16: {
        err = cbor_encode_uint(map_encoder, *(const uint16_t *)entries_table[index].value_source);
        break;
      }
      case UINT32: {
        err = cbor_encode_uint(map_encoder, *(const uint32_t *)entries_table[index].value_source);
        break;
      }
      case UINT64: {
        err = cbor_encode_uint(map_encoder, *(const uint64_t *)entries_table[index].value_source);
        break;
      }

      case FLOAT: {
        err = cbor_encode_float(map_encoder, *(const float *)entries_table[index].value_source);
        break;
      }

      case DOUBLE: {
        err = cbor_encode_double(map_encoder, *(const double *)entries_table[index].value_source);
        break;
      }

      default: {
        err = CborErrorUnsupportedType;
        bm_debug("Unsupported value type for key: %s\n", entries_table[index].key);
        break;
      }

      if (err != CborNoError) {
        bm_debug("Failed to encode value for key: %s\n", entries_table[index].key);
        if (err != CborErrorOutOfMemory) {
          // exit the loop and return, out of memory is an acceptable error
          break;
        }
      }
    }
  }
  return err;
}

