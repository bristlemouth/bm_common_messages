#include "power_battery_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"

CborError PowerBatteryMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size, size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size, PowerBatteryMsg::NUM_FIELDS);

  // sensor_header_msg
  err = SensorHeaderMsg::encode(map_encoder, d.header);
  if (err != CborNoError) {
    bm_debug("SensorHeaderMsg::encode failed: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  check_and_encode_key(err, encode_key_value_uint8(&map_encoder, PowerReadingMsg::POWER_READING_TYPE, d.power_reading_type));
  check_and_encode_key(err, encode_key_value_uint8(&map_encoder, PowerReadingMsg::STATUS, d.status));
  check_and_encode_key(err, encode_key_value_double(&map_encoder, PowerReadingMsg::STATUS, d.voltage_v));
  check_and_encode_key(err, encode_key_value_double(&map_encoder, PowerReadingMsg::STATUS, d.current_a));
  check_and_encode_key(err, encode_key_value_double(&map_encoder, PowerReadingMsg::STATUS, d.charge_ah));
  check_and_encode_key(err, encode_key_value_double(&map_encoder, PowerReadingMsg::STATUS, d.capacity_ah));
  check_and_encode_key(err, encode_key_value_double(&map_encoder, PowerReadingMsg::STATUS, d.percentage));
  check_and_encode_key(err, encode_key_value_uint8(&map_encoder, PowerReadingMsg::STATUS, d.battery_status));
  check_and_encode_key(err, encode_key_value_uint8(&map_encoder, PowerReadingMsg::STATUS, d.battery_health));

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

CborError PowerBatteryMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {


}
