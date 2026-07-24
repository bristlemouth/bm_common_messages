#include "metrics_reply_msg.h"

CborError metrics_reply_encode(const MetricsReplyData *d, uint8_t *cbor_buffer,
                               size_t size, size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder, data_map, comp_map;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                               METRICS_REPLY_NUM_FIELDS);

  check_and_encode_key(err, encode_key_value_uint8(&map_encoder, "version", d->version));
  check_and_encode_key(err, encode_key_value_uint64(&map_encoder, "node_id", d->node_id));
  check_and_encode_key(err, encode_key_value_uint32(&map_encoder, "uptime_ms", d->uptime_ms));

  // "data": { "<component key>": { <flat fields> }, ... }
  check_and_encode_key(err, cbor_encode_text_stringz(&map_encoder, "data"));
  check_and_encode_key(err, cbor_encoder_create_map(&map_encoder, &data_map,
                                                     d->num_components));
  for (size_t i = 0; i < d->num_components; i++) {
    const MetricsComponent *c = &d->components[i];
    check_and_encode_key(err, cbor_encode_text_stringz(&data_map, c->key));
    check_and_encode_key(err, cbor_encoder_create_map(&data_map, &comp_map,
                                                       c->num_fields));
    check_and_encode_key(err, bm_encode_fields_from_table(&comp_map, c->fields,
                                                          c->num_fields));
    check_and_encode_key(err, cbor_encoder_close_container(&data_map, &comp_map));
  }
  check_and_encode_key(err, cbor_encoder_close_container(&map_encoder, &data_map));

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

CborError metrics_reply_decode(const uint8_t *cbor_buffer, size_t size,
                                MetricsReplyDecode *out) {
  CborParser parser;
  CborValue map, value, data_map, comp, field;
  CborError err;
  
  err = decoder_message_enter(&map, &value, &parser, (uint8_t *)cbor_buffer,
                              size, METRICS_REPLY_NUM_FIELDS);

  /* metadata, decoded in wire order */
  check_and_decode_key(err, decode_key_value_uint8(&out->version, &value, "version"));
  check_and_decode_key(err, decode_key_value_uint64(&out->node_id, &value, "node_id"));
  check_and_decode_key(err, decode_key_value_uint32(&out->uptime_ms, &value, "uptime_ms"));
  if (!check_acceptable_decode_errors(err)) {
    return err;
  }

  // "data": { "<component>": { ...flat fields... }, ... }
  if (!cbor_value_is_text_string(&value)) {
    return CborErrorIllegalType;
  }
  if ((err = cbor_value_advance(&value)) != CborNoError) {
    return err;
  }
  if (!cbor_value_is_map(&value)) {
    return CborErrorIllegalType;
  }
  data_map = value;

  for (size_t i = 0; i < out->num_components; i++) {
    const MetricsComponentDecode *c = &out->components[i];
    if ((err = cbor_value_map_find_value(&data_map, c->key, &comp)) != CborNoError) {
      return err;
    }
    if (!cbor_value_is_valid(&comp)) {
      continue; /* component not present in this reply */
    }
    if (!cbor_value_is_map(&comp)) {
      return CborErrorIllegalType;
    }
    if ((err = cbor_value_enter_container(&comp, &field)) != CborNoError) {
      return err;
    }
    err = bm_decode_fields_from_table(&field, c->fields, c->num_fields);
    if (err != CborNoError && err != CborErrorUnsupportedType) {
      return err;
    }
  }
  return CborNoError;
}
