#include <stdint.h>
#include "cbor.h"

// Renamed version of the generated code to avoid conflicts
CborError sensor_header_encode_generated(CborEncoder * const map_encoder,
                                         const uint32_t version,
                                         const uint64_t reading_time_utc_ms,
                                         const uint64_t reading_uptime_millis,
                                         const uint64_t sensor_reading_time_ms) {
    CborError err = CborNoError;

    do {
        // version
        err = cbor_encode_text_stringz(map_encoder, "version");
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, version);
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // reading_time_utc_ms
        err = cbor_encode_text_stringz(map_encoder, "reading_time_utc_ms");
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, reading_time_utc_ms);
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // reading_uptime_millis
        err = cbor_encode_text_stringz(map_encoder, "reading_uptime_millis");
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, reading_uptime_millis);
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // sensor_reading_time_ms
        err = cbor_encode_text_stringz(map_encoder, "sensor_reading_time_ms");
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, sensor_reading_time_ms);
        if (err != CborNoError) {
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}