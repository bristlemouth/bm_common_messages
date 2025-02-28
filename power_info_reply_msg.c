#include "power_info_reply_msg.h"
#include "bm_messages_helper.h"

CborError power_info_reply_encode(PowerInfoReplyData *d, uint8_t *cbor_buffer,
                                  size_t size, size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                       power_info_reply_msg_num_fields);

  check_and_encode_key(
      err, encode_key_value_uint32(&map_encoder, "total_on_s", d->total_on_s));
  check_and_encode_key(
      err,
      encode_key_value_uint32(&map_encoder, "remaining_on_s", d->remaining_on_s));
  check_and_encode_key(
      err,
      encode_key_value_uint32(&map_encoder, "upcoming_off_s", d->upcoming_off_s));
  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

CborError power_info_reply_decode(PowerInfoReplyData *d,
                                  const uint8_t *cbor_buffer, size_t size) {
  CborError err;
  CborParser parser;
  CborValue map, value;

  err = decoder_message_enter(&map, &value, &parser, (uint8_t *)cbor_buffer, size,
                      power_info_reply_msg_num_fields);

  check_and_decode_key(
      err, decode_key_value_uint32(&d->total_on_s, &value, "total_on_s"));
  check_and_decode_key(
      err, decode_key_value_uint32(&d->remaining_on_s, &value, "remaining_on_s"));
  check_and_decode_key(
      err, decode_key_value_uint32(&d->upcoming_off_s, &value, "upcoming_off_s"));

  if (check_acceptable_decode_errors(err)) {
    err = decoder_message_leave(&value, &map);
  }

  return err;
}
