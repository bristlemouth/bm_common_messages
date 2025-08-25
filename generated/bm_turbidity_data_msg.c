#include "bm_turbidity_data_msg.h"
#include "bm_config.h"

CborError bm_turbidity_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double s_signal,
                               const double r_signal) {
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

        // s_signal
        err = cbor_encode_text_stringz(map_encoder, "s_signal");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for s_signal key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, s_signal);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for s_signal value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // r_signal
        err = cbor_encode_text_stringz(map_encoder, "r_signal");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for r_signal key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, r_signal);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for r_signal value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError bm_turbidity_data_decode(                               sensor_header_t * const header_p,
                               double * const s_signal_p,
                               double * const r_signal_p,
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

        // s_signal
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
        *s_signal_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // r_signal
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
        *r_signal_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
