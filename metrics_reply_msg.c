#include "metrics_reply_msg.h"

CborError metrics_reply_encode(const MetricsReplyData *d, uint8_t *cbor_buffer,
                               size_t size, size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder, data_map, comp_map;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                               METRICS_REPLY_NUM_FIELDS);

  check_and_encode_key(err, encode_key_value_uint8(&map_encoder, "mv", d->version));
  check_and_encode_key(err, cbor_encode_text_stringz(&map_encoder, "node"));
  check_and_encode_key(err, cbor_encode_uint(&map_encoder, d->node_id));
  check_and_encode_key(err, encode_key_value_uint32(&map_encoder, "up", d->uptime_ms));

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

static CborError metrics_decode_uint(const CborValue *value, uint64_t *out) {
  if (!cbor_value_is_unsigned_integer(value)) {
    return CborErrorIllegalType;
  }
  return cbor_value_get_uint64(value, out);
}

CborError metrics_reply_decode(const uint8_t *cbor_buffer, size_t size,
                               const MetricsReplyDecode *out) {
  CborParser parser;
  CborValue map, value, data_map, comp, field;
  CborError err;
  uint64_t tmp;

  err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map);
  if (err != CborNoError) {
    return err;
  }
  if (!cbor_value_is_map(&map)) {
    return CborErrorIllegalType;
  }

  if (out->version) {
    if ((err = cbor_value_map_find_value(&map, "mv", &value)) != CborNoError) {
      return err;
    }
    if ((err = metrics_decode_uint(&value, &tmp)) != CborNoError) {
      return err;
    }
    *out->version = (uint8_t)tmp;
  }
  if (out->node_id) {
    if ((err = cbor_value_map_find_value(&map, "node", &value)) != CborNoError) {
      return err;
    }
    if ((err = metrics_decode_uint(&value, out->node_id)) != CborNoError) {
      return err;
    }
  }
  if (out->uptime_ms) {
    if ((err = cbor_value_map_find_value(&map, "up", &value)) != CborNoError) {
      return err;
    }
    if ((err = metrics_decode_uint(&value, &tmp)) != CborNoError) {
      return err;
    }
    *out->uptime_ms = (uint32_t)tmp;
  }

  if ((err = cbor_value_map_find_value(&map, "data", &data_map)) != CborNoError) {
    return err;
  }
  if (!cbor_value_is_map(&data_map)) {
    return CborErrorIllegalType;
  }

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
