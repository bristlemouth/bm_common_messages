#include "pme_wipe_msg.h"
#include "bm_config.h"

CborError pme_wipe_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double wipe_current_mean_ma,
                               const double wipe_duration_s) {
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

        // wipe_current_mean_ma
        err = cbor_encode_text_stringz(map_encoder, "wipe_current_mean_ma");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for wipe_current_mean_ma key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, wipe_current_mean_ma);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for wipe_current_mean_ma value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // wipe_duration_s
        err = cbor_encode_text_stringz(map_encoder, "wipe_duration_s");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for wipe_duration_s key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, wipe_duration_s);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for wipe_duration_s value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError pme_wipe_decode(                               sensor_header_t * const header_p,
                               double * const wipe_current_mean_ma_p,
                               double * const wipe_duration_s_p,
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

        // wipe_current_mean_ma
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
        *wipe_current_mean_ma_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // wipe_duration_s
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
        *wipe_duration_s_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
