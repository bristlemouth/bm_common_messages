#include "barometric_pressure_data_msg.h"
#include "bm_config.h"

CborError barometric_pressure_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double barometric_pressure_mbar) {
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

        // barometric_pressure_mbar
        err = cbor_encode_text_stringz(map_encoder, "barometric_pressure_mbar");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for barometric_pressure_mbar key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, barometric_pressure_mbar);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for barometric_pressure_mbar value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError barometric_pressure_data_decode(                               sensor_header_t * const header_p,
                               double * const barometric_pressure_mbar_p,
                               CborValue * const map) {
    CborError err = CborNoError;

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

        // barometric_pressure_mbar
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
        *barometric_pressure_mbar_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
