#include "power_reading_averages_msg.h"
#include "bm_config.h"
#include "bm_messages_helper.h"

CborError PowerReadingAveragesMsg::encode(Data &d, uint8_t *cbor_buffer, size_t size,
                                          size_t *encoded_len) {
  CborError err;
  CborEncoder encoder, map_encoder;

  err = encoder_message_create(&encoder, &map_encoder, cbor_buffer, size,
                               PowerReadingAveragesMsg::NUM_FIELDS);

  // sensor_header_msg
  err = SensorHeaderMsg::encode(map_encoder, d.header);
  if (err != CborNoError) {
    bm_debug("SensorHeaderMsg::encode failed: %d\n", err);
    if (err != CborErrorOutOfMemory) {
      return err;
    }
  }

  check_and_encode_key(err,
                       encode_key_value_uint8(&map_encoder, PowerReadingMsg::POWER_READING_TYPE,
                                              d.power_reading_type));
  check_and_encode_key(err,
                       encode_key_value_uint8(&map_encoder, PowerReadingMsg::STATUS, d.status));
  check_and_encode_key(err, encode_key_value_uint32(&map_encoder,
                                                    PowerReadingAveragesMsg::NUM_SAMPLES,
                                                    d.num_samples));
  check_and_encode_key(
      err,
      encode_key_value_double(&map_encoder, PowerReadingAveragesMsg::AVERAGING_WINDOW_LENGTH_S,
                              d.averaging_window_length_s));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_AVG,
                                                    d.voltage_v_avg));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_MIN,
                                                    d.voltage_v_min));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_MAX,
                                                    d.voltage_v_max));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_STDEV,
                                                    d.voltage_v_stdev));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::CURRENT_A_AVG,
                                                    d.current_a_avg));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::CURRENT_A_MIN,
                                                    d.current_a_min));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::CURRENT_A_MAX,
                                                    d.current_a_max));
  check_and_encode_key(err, encode_key_value_double(&map_encoder,
                                                    PowerReadingAveragesMsg::CURRENT_A_STDEV,
                                                    d.current_a_stdev));

  if (check_acceptable_encode_errors(err)) {
    err = encoder_message_finish(&encoder, &map_encoder);
    *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
  }

  encoder_message_check_memory(&encoder, err);

  return err;
}

CborError PowerReadingAveragesMsg::decode(Data &d, const uint8_t *cbor_buffer, size_t size) {
  CborParser parser;
  CborValue map, value;
  CborError err;

  // this does not decode the header
  err = decoder_message_enter(&map, &value, &parser, (uint8_t *)cbor_buffer, size,
                              PowerReadingAveragesMsg::NUM_FIELDS);
  if (err != CborNoError) {
    return err;
  }

  // decode header
  err = SensorHeaderMsg::decode(value, d.header);
  if (err != CborNoError) {
    return err;
  }

  check_and_decode_key(err, decode_key_value_uint8((uint8_t *)&d.power_reading_type, &value,
                                                   PowerReadingMsg::POWER_READING_TYPE));
  check_and_decode_key(err, decode_key_value_uint8(&d.status, &value, PowerReadingMsg::STATUS));
  check_and_decode_key(err, decode_key_value_uint32(&d.num_samples, &value,
                                                    PowerReadingAveragesMsg::NUM_SAMPLES));
  check_and_decode_key(
      err, decode_key_value_double(&d.averaging_window_length_s, &value,
                                   PowerReadingAveragesMsg::AVERAGING_WINDOW_LENGTH_S));
  check_and_decode_key(err, decode_key_value_double(&d.voltage_v_avg, &value,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_AVG));
  check_and_decode_key(err, decode_key_value_double(&d.voltage_v_min, &value,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_MIN));
  check_and_decode_key(err, decode_key_value_double(&d.voltage_v_max, &value,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_MAX));
  check_and_decode_key(err, decode_key_value_double(&d.voltage_v_stdev, &value,
                                                    PowerReadingAveragesMsg::VOLTAGE_V_STDEV));
  check_and_decode_key(err, decode_key_value_double(&d.current_a_avg, &value,
                                                    PowerReadingAveragesMsg::CURRENT_A_AVG));
  check_and_decode_key(err, decode_key_value_double(&d.current_a_min, &value,
                                                    PowerReadingAveragesMsg::CURRENT_A_MIN));
  check_and_decode_key(err, decode_key_value_double(&d.current_a_max, &value,
                                                    PowerReadingAveragesMsg::CURRENT_A_MAX));
  check_and_decode_key(err, decode_key_value_double(&d.current_a_stdev, &value,
                                                    PowerReadingAveragesMsg::CURRENT_A_STDEV));

  if (check_acceptable_decode_errors(err)) {
    err = decoder_message_leave(&value, &map);
  }

  return err;
}
