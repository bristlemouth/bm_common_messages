#include "metrics_reply_msg.h"
#include "bm_messages_helper.h"

CborError metrics_reply_encode(const MetricsReplyData *d, uint8_t *cbor_buffer,
                               size_t size, size_t *encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder, port_array, port_map;

    err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                                METRICS_REPLY_NUM_FIELDS);

    check_and_encode_key(err, encode_key_value_uint8(&map_encoder, "v", d->version));
    check_and_encode_key(err, cbor_encode_text_stringz(&map_encoder, "node"));
    check_and_encode_key(err, cbor_encode_uint(&map_encoder, d->node_id));
    check_and_encode_key(err, encode_key_value_uint32(&map_encoder, "up", d->uptime_ms));

    // "p": [ {port0}, {port1}, ... ] 
    check_and_encode_key(err, cbor_encode_text_stringz(&map_encoder, "p"));
    check_and_encode_key(err, cbor_encoder_create_array(&map_encoder, &port_array, d->num_ports));
    for (uint8_t i = 0; i < d->num_ports; i++) {
        check_and_encode_key(err, cbor_encoder_create_map(&port_array, &port_map, METRICS_PORT_NUM_FIELDS));
        check_and_encode_key(err, encode_key_value_uint32(&port_map, "mse", d->ports[i].mse_val));
        check_and_encode_key(err, encode_key_value_uint8(&port_map, "sqi", d->ports[i].sqi));
        check_and_encode_key(err, encode_key_value_uint8(&port_map, "lq",  d->ports[i].link_quality));
        check_and_encode_key(err, encode_key_value_uint32(&port_map, "rxe", d->ports[i].rx_err_count));
        check_and_encode_key(err, encode_key_value_uint32(&port_map, "sye", d->ports[i].symbol_err_count));
        check_and_encode_key(err, cbor_encoder_close_container(&port_array, &port_map));
    }
    check_and_encode_key(err, cbor_encoder_close_container(&map_encoder, &port_array));

    if (check_acceptable_encode_errors(err)) {
        err = encoder_message_finish(&encoder, &map_encoder);
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
    }

    encoder_message_check_memory(&encoder, err);

    return err;
}
