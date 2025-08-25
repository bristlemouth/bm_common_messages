#include "sys_info_svc_reply_msg.h"
#include "bm_config.h"

CborError sys_info_svc_reply_encode(CborEncoder * const map_encoder,
                               const uint64_t node_id,
                               const uint32_t git_sha,
                               const uint32_t sys_config_crc,
                               const uint32_t app_name_strlen,
                               const char* app_name) {
    CborError err = CborNoError;

    do {
        // node_id
        err = cbor_encode_text_stringz(map_encoder, "node_id");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for node_id key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, node_id);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for node_id value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // git_sha
        err = cbor_encode_text_stringz(map_encoder, "git_sha");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for git_sha key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, git_sha);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for git_sha value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // sys_config_crc
        err = cbor_encode_text_stringz(map_encoder, "sys_config_crc");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for sys_config_crc key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, sys_config_crc);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for sys_config_crc value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // app_name_strlen
        err = cbor_encode_text_stringz(map_encoder, "app_name_strlen");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for app_name_strlen key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, app_name_strlen);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for app_name_strlen value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // app_name
        err = cbor_encode_text_stringz(map_encoder, "app_name");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for app_name key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_text_stringz(map_encoder, app_name);
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for app_name value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError sys_info_svc_reply_decode(                               uint64_t * const node_id_p,
                               uint32_t * const git_sha_p,
                               uint32_t * const sys_config_crc_p,
                               uint32_t * const app_name_strlen_p,
                               char ** const app_name_p,
                               CborValue * const map) {
    CborError err = CborNoError;

    uint64_t tmp_uint64;

    do {
        // node_id
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
        *node_id_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // git_sha
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
        *git_sha_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // sys_config_crc
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
        *sys_config_crc_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // app_name_strlen
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
        *app_name_strlen_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // app_name
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        size_t tmp_string_len;
        err = cbor_value_calculate_string_length(map, &tmp_string_len);
        if (err != CborNoError) {
            break;
        }
        // Note: Caller must allocate memory for string
        // This is a limitation of the low-level C API
        err = CborErrorUnknownType; // TODO: Implement string memory allocation
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
