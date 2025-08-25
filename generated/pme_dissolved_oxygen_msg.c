#include "pme_dissolved_oxygen_msg.h"
#include "bm_config.h"

CborError pme_dissolved_oxygen_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double temperature_deg_c,
                               const double do_mg_per_l,
                               const double quality,
                               const double do_saturation_pct,
                               const float salinity_ppt) {
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

        // do_mg_per_l
        err = cbor_encode_text_stringz(map_encoder, "do_mg_per_l");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for do_mg_per_l key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, do_mg_per_l);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for do_mg_per_l value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // quality
        err = cbor_encode_text_stringz(map_encoder, "quality");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for quality key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, quality);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for quality value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // do_saturation_pct
        err = cbor_encode_text_stringz(map_encoder, "do_saturation_pct");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for do_saturation_pct key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, do_saturation_pct);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for do_saturation_pct value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // salinity_ppt
        err = cbor_encode_text_stringz(map_encoder, "salinity_ppt");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for salinity_ppt key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_float(map_encoder, salinity_ppt);
        if (err != CborNoError) {
            bm_debug("cbor_encode_float failed for salinity_ppt value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError pme_dissolved_oxygen_decode(                               sensor_header_t * const header_p,
                               double * const temperature_deg_c_p,
                               double * const do_mg_per_l_p,
                               double * const quality_p,
                               double * const do_saturation_pct_p,
                               float * const salinity_ppt_p,
                               CborValue * const map) {
    CborError err = CborNoError;

    float tmp_float;
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

        // temperature_deg_c
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

        // do_mg_per_l
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
        *do_mg_per_l_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // quality
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
        *quality_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // do_saturation_pct
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
        *do_saturation_pct_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // salinity_ppt
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_float(map, &tmp_float);
        if (err != CborNoError) {
            break;
        }
        *salinity_ppt_p = tmp_float;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
