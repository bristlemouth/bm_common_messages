#include "bm_rbr_data_msg.h"
#include "bm_config.h"

CborError bm_rbr_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const uint32_t sensor_type,
                               const double temperature_deg_c,
                               const double pressure_deci_bar) {
    CborError err = CborNoError;

    do {
        // header
        err = cbor_encode_text_stringz(map_encoder, "header");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for header key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = sensor_header_encode(map_encoder, header);
        if (err != CborNoError) {
            bm_debug("sensor_header_encode failed for header: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // sensor_type
        err = cbor_encode_text_stringz(map_encoder, "sensor_type");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for sensor_type key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, sensor_type);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for sensor_type value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // temperature_deg_c
        err = cbor_encode_text_stringz(map_encoder, "temperature_deg_c");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for temperature_deg_c key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, temperature_deg_c);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for temperature_deg_c value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // pressure_deci_bar
        err = cbor_encode_text_stringz(map_encoder, "pressure_deci_bar");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for pressure_deci_bar key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, pressure_deci_bar);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for pressure_deci_bar value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError bm_rbr_data_decode(                               sensor_header_t * const header_p,
                               uint32_t * const sensor_type_p,
                               double * const temperature_deg_c_p,
                               double * const pressure_deci_bar_p,
                               CborValue * const map) {
    CborError err = CborNoError;

    uint64_t tmp_uint64;
    double tmp_double;

    do {
        // header
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = sensor_header_decode(header_p, map);
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // sensor_type
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
        *sensor_type_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // temperature_deg_c (optional)
        // Note: Optional fields are encoded unconditionally in this implementation
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_double(map, &tmp_double);
        if (err != CborNoError) {
            break;
        }
        *temperature_deg_c_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // pressure_deci_bar (optional)
        // Note: Optional fields are encoded unconditionally in this implementation
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_double(map, &tmp_double);
        if (err != CborNoError) {
            break;
        }
        *pressure_deci_bar_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
