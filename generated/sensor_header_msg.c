#include "sensor_header_msg.h"
#include "bm_config.h"

CborError sensor_header_encode(CborEncoder * const map_encoder,
                               const uint32_t version,
                               const uint64_t reading_time_utc_ms,
                               const uint64_t reading_uptime_millis,
                               const uint64_t sensor_reading_time_ms) {
    CborError err = CborNoError;

    do {
        // version
        err = cbor_encode_text_stringz(map_encoder, "version");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for version key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, version);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for version value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // reading_time_utc_ms
        err = cbor_encode_text_stringz(map_encoder, "reading_time_utc_ms");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for reading_time_utc_ms key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, reading_time_utc_ms);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for reading_time_utc_ms value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // reading_uptime_millis
        err = cbor_encode_text_stringz(map_encoder, "reading_uptime_millis");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for reading_uptime_millis key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, reading_uptime_millis);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for reading_uptime_millis value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // sensor_reading_time_ms
        err = cbor_encode_text_stringz(map_encoder, "sensor_reading_time_ms");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for sensor_reading_time_ms key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, sensor_reading_time_ms);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for sensor_reading_time_ms value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError sensor_header_decode(                               uint32_t * const version_p,
                               uint64_t * const reading_time_utc_ms_p,
                               uint64_t * const reading_uptime_millis_p,
                               uint64_t * const sensor_reading_time_ms_p,
                               CborValue * const map) {
    CborError err = CborNoError;

    uint64_t tmp_uint64;

    do {
        // version
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_uint64(map, &tmp_uint64);
        if (err != CborNoError) {
            break;
        }
        *version_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // reading_time_utc_ms
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_uint64(map, &tmp_uint64);
        if (err != CborNoError) {
            break;
        }
        *reading_time_utc_ms_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // reading_uptime_millis
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_uint64(map, &tmp_uint64);
        if (err != CborNoError) {
            break;
        }
        *reading_uptime_millis_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // sensor_reading_time_ms
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_uint64(map, &tmp_uint64);
        if (err != CborNoError) {
            break;
        }
        *sensor_reading_time_ms_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
