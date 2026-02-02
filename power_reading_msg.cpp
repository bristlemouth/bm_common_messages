#include "power_reading_msg.h"
#include "bm_config.h"

CborError PowerReadingMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;
  cbor_encoder_init(&encoder, cbor_buffer, size, 0);

  // Here we need to begin the more flexible encoding / decoding, where if we are missing a value
  // from the struct... actually the encoding may need to be rigid, while decoding can
  // be more flexible... otherwise we would need to make the inputs optional and a ton of them?



}