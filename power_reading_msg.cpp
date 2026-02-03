#include "power_reading_msg.h"
#include "bm_config.h"

CborError PowerReadingMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                  size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  do {
    err = cbor_encoder_create_map(&encoder, &map_encoder, NUM_FIELDS);
    if (err != CborNoError) {
      bm_debug("cbor_encoer_create_map failed: %d\n", err);
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

    // power_reading_type
    err = cbor_encode_text_stringz(&map_encoder, "power_reading_type");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for power_reading_type key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_uint(&map_encoder, d.power_reading_type);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for power_reading_type value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // voltage_v
    err = cbor_encode_text_stringz(&map_encoder, "voltage_v");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for voltage_v key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.voltage_v);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for voltage_v value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // current_ma
    err = cbor_encode_text_stringz(&map_encoder, "current_ma");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for current_ma key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.current_ma);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for current_ma value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // status
    err = cbor_encode_text_stringz(&map_encoder, "status");
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for status key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_uint(&map_encoder, d.status);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for status value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encoder_close_container(&encoder, &map_encoder);
    if (err == CborNoError) {
      *encoded_len = cbor_encoder_get_buffer_size(&encoder, &map_encoder);
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

CborError PowerReadingMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {

  // TODO - we will want this to be the more flexible approach!
}