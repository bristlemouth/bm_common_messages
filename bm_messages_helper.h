#ifndef __BM_MESSAGES_HELPER_H__
#define __BM_MESSAGES_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cbor.h"

#define max_key_len (64)

#define check_acceptable_encode_errors(e)                                      \
  (e == CborNoError || e == CborErrorOutOfMemory)
#define check_and_encode_key(e, f)                                             \
  if (check_acceptable_encode_errors(e)) {                                     \
    e = f;                                                                     \
  }
#define check_acceptable_decode_errors(e) (e == CborNoError)
#define check_and_decode_key(e, f)                                             \
  if (check_acceptable_decode_errors(e)) {                                     \
    e = f;                                                                     \
  }

CborError encoder_message_create(CborEncoder *encoder, CborEncoder *map_encoder,
                                 uint8_t *cbor_buffer, size_t size,
                                 size_t num_fields);
CborError encode_key_value_float(CborEncoder *map_encoder, const char *name,
                                 const float value);
CborError encode_key_value_double(CborEncoder *map_encoder, const char *name,
                                 const double value);
CborError encode_key_value_uint8(CborEncoder *map_encoder, const char *name,
                                 const uint8_t value);
CborError encode_key_value_uint32(CborEncoder *map_encoder, const char *name,
                                  const uint32_t value);
CborError encode_key_value_string(CborEncoder *map_encoder, const char *name,
                                  const char *value, const size_t len);
CborError encode_key_value_bytes(CborEncoder *map_encoder, const char *name,
                                 const unsigned char *value, const size_t len);
CborError encoder_message_finish(CborEncoder *encoder,
                                 CborEncoder *map_encoder);
void encoder_message_check_memory(CborEncoder *encoder, CborError err);

CborError decoder_message_enter(CborValue *map, CborValue *decode_value,
                                CborParser *parser, uint8_t *cbor_buffer,
                                size_t size, size_t num_fields);
CborError decode_key_value_float(float *out, CborValue *value,
                                 const char *key_expected);
CborError decode_key_value_double(double *out, CborValue *value,
                                 const char *key_expected);
CborError decode_key_value_uint8(uint8_t *out, CborValue *value,
                                 const char *key_expected);
CborError decode_key_value_uint32(uint32_t *out, CborValue *value,
                                  const char *key_expected);
CborError decode_key_value_string(char **out, size_t *len, CborValue *value,
                                  const char *key_expected);
CborError decode_key_value_bytes(uint8_t **out, size_t *len, CborValue *value,
                                 const char *key_expected);
CborError decoder_message_leave(CborValue *value, CborValue *map);

#ifdef __cplusplus
}
#endif

#endif
