#include "sensor_header_msg.h"
#include "FreeRTOS.h"

CborError SensorHeaderMsg::encode(CborEncoder &map_encoder, Data &d) {
  CborError err = CborNoError;

  do {
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
    err = cbor_encode_text_stringz(&map_encoder, "sensor_reading_time_s");
    if (err != CborNoError) {
      printf("cbor_encode_text_stringz failed for sensor_reading_time_s key: %d\n", err);
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
  } while (0);

  return err;
}

CborError SensorHeaderMsg::decode(CborValue &map, Data &d) {
  CborError err = CborNoError;

  do {
    // version
    if (!cbor_value_is_text_string(&map)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }
    uint64_t tmp_uint64;
    err = cbor_value_get_uint64(&map, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.version = tmp_uint64;
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }

    // reading_time_utc_s
    if (!cbor_value_is_text_string(&map)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&map, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.reading_time_utc_s = tmp_uint64;
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }

    // reading_uptime_millis
    if (!cbor_value_is_text_string(&map)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&map, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.reading_uptime_millis = tmp_uint64;
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }

    // sensor_reading_time_s
    if (!cbor_value_is_text_string(&map)) {
      err = CborErrorIllegalType;
      printf("expected string key but got something else\n");
      break;
    }
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }
    err = cbor_value_get_uint64(&map, &tmp_uint64);
    if (err != CborNoError) {
      break;
    }
    d.sensor_reading_time_s = tmp_uint64;
    err = cbor_value_advance(&map);
    if (err != CborNoError) {
      break;
    }
  } while (0);

  return err;
}
