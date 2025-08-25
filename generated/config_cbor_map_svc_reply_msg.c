#include "config_cbor_map_svc_reply_msg.h"
#include "bm_config.h"

CborError config_cbor_map_svc_reply_encode(CborEncoder * const map_encoder,
                               const uint64_t node_id,
                               const uint32_t partition_id,
                               const bool success,
                               const uint32_t cbor_encoded_map_len,
                               const uint8_t* cbor_data) {
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

        // partition_id
        err = cbor_encode_text_stringz(map_encoder, "partition_id");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for partition_id key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, partition_id);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for partition_id value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // success
        err = cbor_encode_text_stringz(map_encoder, "success");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for success key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_boolean(map_encoder, success);
        if (err != CborNoError) {
            bm_debug("cbor_encode_boolean failed for success value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // cbor_encoded_map_len
        err = cbor_encode_text_stringz(map_encoder, "cbor_encoded_map_len");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for cbor_encoded_map_len key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_uint(map_encoder, cbor_encoded_map_len);
        if (err != CborNoError) {
            bm_debug("cbor_encode_uint failed for cbor_encoded_map_len value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

        // cbor_data
        err = cbor_encode_text_stringz(map_encoder, "cbor_data");
        if (err != CborNoError) {
            bm_debug("cbor_encode_text_stringz failed for cbor_data key: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }
        err = cbor_encode_byte_string(map_encoder, cbor_data, cbor_encoded_map_len);
        if (err != CborNoError) {
            bm_debug("cbor_encode_byte_string failed for cbor_data value: %d\n", err);
            if (err != CborErrorOutOfMemory) {
                break;
            }
        }

    } while (0);

    return err;
}

CborError config_cbor_map_svc_reply_decode(                               uint64_t * const node_id_p,
                               uint32_t * const partition_id_p,
                               bool * const success_p,
                               uint32_t * const cbor_encoded_map_len_p,
                               uint8_t* * const cbor_data_p,
                               CborValue * const map) {
    CborError err = CborNoError;

    uint64_t tmp_uint64;
    bool tmp_bool;

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

        // partition_id
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
        *partition_id_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // success
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        err = cbor_value_get_boolean(map, &tmp_bool);
        if (err != CborNoError) {
            break;
        }
        *success_p = tmp_bool;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // cbor_encoded_map_len
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
        *cbor_encoded_map_len_p = tmp_uint64;
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

        // cbor_data
        if (!cbor_value_is_text_string(map)) {
            err = CborErrorIllegalType;
            bm_debug("expected string key but got something else\n");
            break;
        }
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }
        size_t tmp_array_len;
        err = cbor_value_calculate_string_length(map, &tmp_array_len);
        if (err != CborNoError) {
            break;
        }
        // Note: Caller must allocate memory for array data
        // This is a limitation of the low-level C API
        err = CborErrorUnknownType; // TODO: Implement array memory allocation
        err = cbor_value_advance(map);
        if (err != CborNoError) {
            break;
        }

    } while (0);

    return err;
}
