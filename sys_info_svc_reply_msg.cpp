#include "sys_info_svc_reply_msg.h"
#include "FreeRTOS.h"

CborError SysInfoSvcReplyMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
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

    // node_id
    err = cbor_encode_text_stringz(&map_encoder, "node_id");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for node_id key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.node_id);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for node_id value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // git_sha
    err = cbor_encode_text_stringz(&map_encoder, "git_sha");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for git_sha key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.git_sha);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for git_sha value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // sys_config_crc
    err = cbor_encode_text_stringz(&map_encoder, "sys_config_crc");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for sys_config_crc key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.sys_config_crc);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for sys_config_crc value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // app_name_strlen
    err = cbor_encode_text_stringz(&map_encoder, "app_name_strlen");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for app_name_strlen key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_uint(&map_encoder, d.app_name_strlen);
    if (err != CborNoError) {
      printf("cbor_encode_uint failed for app_name_strlen value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // app_name
    err = cbor_encode_text_stringz(&map_encoder, "app_name");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for app_name key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    err = cbor_encode_text_stringz(&map_encoder, d.app_name);
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for app_name value: %d\n", err);
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

// Allocates memory for app_name. Caller is responsible for freeing it if d.app_name != NULL.
CborError SysInfoSvcReplyMsg::decode(SysInfoSvcReplyMsg::Data &d, const uint8_t *cbor_buffer,
                                     size_t size) {
  d.app_name = NULL;
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
    if (num_fields != SysInfoSvcReplyMsg::NUM_FIELDS) {
      err = CborErrorUnknownLength;
      printf("expected %zu fields but got %zu\n", SysInfoSvcReplyMsg::NUM_FIELDS, num_fields);
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
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&value, &d.node_id);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // git_sha
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
    d.git_sha = tmp_uint64;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // sys_config_crc
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
    d.sys_config_crc = tmp_uint64;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // app_name_strlen
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
    d.app_name_strlen = tmp_uint64;
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }

    // app_name
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&value);
    if (err != CborNoError) {
      break;
    }
    if (!cbor_value_is_text_string(&value)) {
      err = CborErrorIllegalType;
      printf("expected string value but got something else\n");
      break;
    }
    size_t buflen = d.app_name_strlen + 1;
    char *buf = static_cast<char *>(pvPortMalloc(sizeof(char) * buflen));
    if (buf == NULL) {
      err = CborErrorOutOfMemory;
      break;
    }
    err = cbor_value_copy_text_string(&value, buf, &buflen, NULL);
    if (err != CborNoError) {
      break;
    }
    d.app_name = buf;
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