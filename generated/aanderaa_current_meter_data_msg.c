#include "aanderaa_current_meter_data_msg.h"
#include "bm_config.h"

CborError aanderaa_current_meter_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double abs_speed_cm_s,
                               const double direction_deg_m,
                               const double north_cm_s,
                               const double east_cm_s,
                               const double heading_deg_m,
                               const double tilt_x_deg,
                               const double tilt_y_deg,
                               const double single_ping_std_cm_s,
                               const double transducer_strength_db,
                               const double ping_count,
                               const double abs_tilt_deg,
                               const double max_tilt_deg,
                               const double std_tilt_deg,
                               const double temperature_deg_c) {
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

        // abs_speed_cm_s
        err = cbor_encode_text_stringz(map_encoder, "abs_speed_cm_s");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for abs_speed_cm_s key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, abs_speed_cm_s);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for abs_speed_cm_s value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // direction_deg_m
        err = cbor_encode_text_stringz(map_encoder, "direction_deg_m");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for direction_deg_m key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, direction_deg_m);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for direction_deg_m value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // north_cm_s
        err = cbor_encode_text_stringz(map_encoder, "north_cm_s");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for north_cm_s key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, north_cm_s);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for north_cm_s value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // east_cm_s
        err = cbor_encode_text_stringz(map_encoder, "east_cm_s");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for east_cm_s key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, east_cm_s);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for east_cm_s value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // heading_deg_m
        err = cbor_encode_text_stringz(map_encoder, "heading_deg_m");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for heading_deg_m key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, heading_deg_m);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for heading_deg_m value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // tilt_x_deg
        err = cbor_encode_text_stringz(map_encoder, "tilt_x_deg");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for tilt_x_deg key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, tilt_x_deg);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for tilt_x_deg value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // tilt_y_deg
        err = cbor_encode_text_stringz(map_encoder, "tilt_y_deg");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for tilt_y_deg key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, tilt_y_deg);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for tilt_y_deg value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // single_ping_std_cm_s
        err = cbor_encode_text_stringz(map_encoder, "single_ping_std_cm_s");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for single_ping_std_cm_s key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, single_ping_std_cm_s);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for single_ping_std_cm_s value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // transducer_strength_db
        err = cbor_encode_text_stringz(map_encoder, "transducer_strength_db");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for transducer_strength_db key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, transducer_strength_db);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for transducer_strength_db value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // ping_count
        err = cbor_encode_text_stringz(map_encoder, "ping_count");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for ping_count key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, ping_count);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for ping_count value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // abs_tilt_deg
        err = cbor_encode_text_stringz(map_encoder, "abs_tilt_deg");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for abs_tilt_deg key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, abs_tilt_deg);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for abs_tilt_deg value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // max_tilt_deg
        err = cbor_encode_text_stringz(map_encoder, "max_tilt_deg");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for max_tilt_deg key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, max_tilt_deg);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for max_tilt_deg value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // std_tilt_deg
        err = cbor_encode_text_stringz(map_encoder, "std_tilt_deg");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for std_tilt_deg key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_double(map_encoder, std_tilt_deg);
        if (err != CborNoError) {
            bm_debug("cbor_encode_double failed for std_tilt_deg value: %d\n", err);
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

    } while (0);

    return err;
}

CborError aanderaa_current_meter_data_decode(                               sensor_header_t * const header_p,
                               double * const abs_speed_cm_s_p,
                               double * const direction_deg_m_p,
                               double * const north_cm_s_p,
                               double * const east_cm_s_p,
                               double * const heading_deg_m_p,
                               double * const tilt_x_deg_p,
                               double * const tilt_y_deg_p,
                               double * const single_ping_std_cm_s_p,
                               double * const transducer_strength_db_p,
                               double * const ping_count_p,
                               double * const abs_tilt_deg_p,
                               double * const max_tilt_deg_p,
                               double * const std_tilt_deg_p,
                               double * const temperature_deg_c_p,
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

        // abs_speed_cm_s
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
        *abs_speed_cm_s_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // direction_deg_m
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
        *direction_deg_m_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // north_cm_s
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
        *north_cm_s_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // east_cm_s
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
        *east_cm_s_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // heading_deg_m
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
        *heading_deg_m_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // tilt_x_deg
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
        *tilt_x_deg_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // tilt_y_deg
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
        *tilt_y_deg_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // single_ping_std_cm_s
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
        *single_ping_std_cm_s_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // transducer_strength_db
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
        *transducer_strength_db_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // ping_count
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
        *ping_count_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // abs_tilt_deg
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
        *abs_tilt_deg_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // max_tilt_deg
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
        *max_tilt_deg_p = tmp_double;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // std_tilt_deg
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
        *std_tilt_deg_p = tmp_double;
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

    } while (0);

    return err;
}
