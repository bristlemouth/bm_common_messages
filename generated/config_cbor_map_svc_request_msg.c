#include "config_cbor_map_svc_request_msg.h"
#include "bm_config.h"

CborError config_cbor_map_svc_request_encode(CborEncoder * const map_encoder,
                               const uint32_t partition_id) {
    CborError err = CborNoError;

    do {
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

    } while (0);

    return err;
}

CborError config_cbor_map_svc_request_decode(                               uint32_t * const partition_id_p,
                               CborValue * const map) {
    CborError err = CborNoError;

    uint64_t tmp_uint64;

    do {
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

    } while (0);

    return err;
}
