#include "aanderaa_conductivity_data_msg.h"
#include "bm_config.h"

CborError aanderaa_conductivity_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double conductivity_ms_cm,
                               const double temperature_deg_c,
                               const double salinity_psu,
                               const double water_density_kg_m3,
                               const double sound_speed_m_s,
                               const float depth_m) {
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

        // conductivity_ms_cm
        err = cbor_encode_text_stringz(map_encoder, "conductivity_ms_cm");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for conductivity_ms_cm key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, conductivity_ms_cm);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for conductivity_ms_cm value: %d\n", err);
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

        // salinity_psu
        err = cbor_encode_text_stringz(map_encoder, "salinity_psu");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for salinity_psu key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, salinity_psu);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for salinity_psu value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // water_density_kg_m3
        err = cbor_encode_text_stringz(map_encoder, "water_density_kg_m3");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for water_density_kg_m3 key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, water_density_kg_m3);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for water_density_kg_m3 value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // sound_speed_m_s
        err = cbor_encode_text_stringz(map_encoder, "sound_speed_m_s");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for sound_speed_m_s key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, sound_speed_m_s);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for sound_speed_m_s value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // depth_m
        err = cbor_encode_text_stringz(map_encoder, "depth_m");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for depth_m key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_float(map_encoder, depth_m);
        if (err != CborNoError) {
            bm_debug("cbor_encode_float failed for depth_m value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError aanderaa_conductivity_data_decode(                               sensor_header_t * const header_p,
                               double * const conductivity_ms_cm_p,
                               double * const temperature_deg_c_p,
                               double * const salinity_psu_p,
                               double * const water_density_kg_m3_p,
                               double * const sound_speed_m_s_p,
                               float * const depth_m_p,
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

        // conductivity_ms_cm
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
        *conductivity_ms_cm_p = tmp_double;
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

        // salinity_psu
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
        *salinity_psu_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // water_density_kg_m3
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
        *water_density_kg_m3_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // sound_speed_m_s
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
        *sound_speed_m_s_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // depth_m
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
        *depth_m_p = tmp_float;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
