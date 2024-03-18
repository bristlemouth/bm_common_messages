#include "device_test_svc_reply_msg.h"
#ifndef CI_TEST
#include "FreeRTOS.h"
#endif // CI_TEST

CborError DeviceTestSvcReplyMsg::encode(Data &d, uint8_t *cbor_buffer,
                                        size_t size, size_t *encoded_len) {
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

    // success
    err = cbor_encode_text_stringz(&map_encoder, "success");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for success key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.success);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for partition_id value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // data_len
    err = cbor_encode_text_stringz(&map_encoder, "data_len");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for data_len key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.data_len);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for data_len value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // data
    err = cbor_encode_text_stringz(&map_encoder, "data");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for data key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_byte_string(&map_encoder, d.data, d.data_len);
    if (err != CborNoError) {
      printf("cbor_encode_byte_string failed for data value: %d\n", err);
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

CborError DeviceTestSvcReplyMsg::decode(Data &d, const uint8_t *cbor_buffer,
                                        size_t size) {
  CborParser parser;
  CborValue map;
  CborError err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);
  uint64_t tmp_uint64;
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
      printf("expected %zu fields but got %zu\n", NUM_FIELDS, num_fields);
      break;
    }

    CborValue value;
    err = cbor_value_enter_container(&map, &value);
    if (err != CborNoError) {
      break;
    }

    // success
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
    d.success = static_cast<uint32_t>(tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // data_len
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
    d.data_len = static_cast<uint32_t>(tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // data
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    if (d.data_len) {
      size_t buflen = d.data_len;
#ifndef CI_TEST
      uint8_t *buf = static_cast<uint8_t *>(pvPortMalloc(buflen));
      configASSERT(buf);
#else
      uint8_t *buf = static_cast<uint8_t *>(malloc(buflen));
#endif // CI_TEST
      err = cbor_value_copy_byte_string(&value, buf, &buflen, NULL);
      d.data = buf;
      if (err != CborNoError) {
        break;
      }
      if (buflen != d.data_len) {
        err = CborErrorIllegalType;
        break;
      }
    } else {
      d.data = NULL;
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