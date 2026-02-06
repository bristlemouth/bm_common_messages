#include "power_reading_averages_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"

CborError PowerReadingAveragesMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
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
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingMsg::POWER_READING_TYPE);
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

    // status
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingMsg::STATUS);
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

    // voltage_v_avg
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::VOLTAGE_V_AVG);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for voltage_v_avg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.voltage_v_avg);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for voltage_v_avg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // voltage_v_min
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::VOLTAGE_V_MIN);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for voltage_v key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.voltage_v_min);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for voltage_v_min value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // voltage_v_max
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::VOLTAGE_V_MAX);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for voltage_v_max key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.voltage_v_max);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for voltage_v_max value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // voltage_v_stdev
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::VOLTAGE_V_STDEV);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for voltage_v_stdev key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.voltage_v_stdev);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for voltage_v_stdev value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // current_a_avg
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::CURRENT_A_AVG);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for current_a_avg key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.current_a_avg);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for current_a_avg value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // current_a_min
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::CURRENT_A_MIN);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for current_a_min key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.current_a_min);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for current_a_min value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    // current_a_max
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::CURRENT_A_MAX);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for current_a_max key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.current_a_max);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for current_a_max value: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }
    // current_a_stdev
    err = cbor_encode_text_stringz(&map_encoder, PowerReadingAveragesMsg::CURRENT_A_STDEV);
    if (err != CborNoError) {
      bm_debug("cbor_encode_text_stringz failed for current_a_stdev key: %d\n", err);
      if (err != CborErrorOutOfMemory) {
        break;
      }
    }

    err = cbor_encode_double(&map_encoder, d.current_a_stdev);
    if (err != CborNoError) {
      bm_debug("cbor_encode_double failed for current_a_stdev value: %d\n", err);
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

CborError PowerReadingAveragesMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map;
  (void) d;
  (void) cbor_buffer;
  (void) size;
  // used to indicate if the message has additional fields we cannot currently decode
  // bool unsupported_type = false;

  CborError err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);

  return err;
}