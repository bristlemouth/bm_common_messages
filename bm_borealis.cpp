#include "bm_borealis.h"
#include "bm_config.h"
#include "bm_messages_helper.h"
#ifndef CI_TEST
#include "bm_os.h"
#endif

#if __has_include ("debug.h")
#include "debug.h"
#else
#define debug_printf printf
#endif

#include <math.h>

/* re-enable some warnings that bm_protocol (inadvertently?) disables as of this writing */
#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wshadow"
#pragma GCC diagnostic warning "-Wformat"

static CborError create_map_and_send_header(CborEncoder * encoder, CborEncoder * map_encoder, SensorHeaderMsg::Data header, const size_t num_fields) {
    CborError err;
    if ((err = cbor_encoder_create_map(encoder, map_encoder, num_fields)) != CborNoError) {
        debug_printf("error: %s: cbor_encoder_create_map failed: %d\r\n", __func__, err);
        if (err != CborErrorOutOfMemory) return err;
    }

    if ((err = SensorHeaderMsg::encode(*map_encoder, header)) != CborNoError)
        debug_printf("error: %s: SensorHeaderMsg::encode failed: %d\r\n", __func__, err);
    return err;
}

CborError borealis_spectrum_data_encode(struct borealis_spectrum_data * d, uint8_t * cbor_buffer, size_t size, size_t * encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder;
    cbor_encoder_init(&encoder, cbor_buffer, size, 0);

    do {
        if ((err = create_map_and_send_header(&encoder, &map_encoder, d->header, BOREALIS_SPECTRUM_MSG_NUM_FIELDS)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_float(&map_encoder, "dt", d->dt)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_float(&map_encoder, "df", d->df)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_uint8(&map_encoder, "bands_per_octave", d->bands_per_octave)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_string(&map_encoder, "spectrum", d->spectrum_as_base64, d->spectrum_length)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encoder_message_finish(&encoder, &map_encoder)) != CborNoError && err != CborErrorOutOfMemory) break;

        if (err != CborNoError) break;

        /* no errors have occurred, return normally */
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
        return CborNoError;
    } while (0);

    encoder_message_check_memory(&encoder, err);

    return err;
}

CborError borealis_levels_encode(struct borealis_levels * d, uint8_t * cbor_buffer, size_t size, size_t * encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder;
    cbor_encoder_init(&encoder, cbor_buffer, size, 0);

    do {
        if ((err = create_map_and_send_header(&encoder, &map_encoder, d->header, BOREALIS_LEVELS_MSG_NUM_FIELDS)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_float(&map_encoder, "dt", d->dt)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_float(&map_encoder, "dt_report", d->dt_report)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_uint8(&map_encoder, "first_band_index", d->first_band_index)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_string(&map_encoder, "levels", d->levels_as_base64, d->levels_length)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encoder_message_finish(&encoder, &map_encoder)) != CborNoError && err != CborErrorOutOfMemory) break;

        if (err != CborNoError) break;

        /* no errors have occurred, return normally */
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
        return CborNoError;
    } while (0);

    encoder_message_check_memory(&encoder, err);

    return err;
}

CborError borealis_recording_status_encode(struct borealis_recording_status * d, uint8_t * cbor_buffer, size_t size, size_t * encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder;
    cbor_encoder_init(&encoder, cbor_buffer, size, 0);

    do {
        if ((err = create_map_and_send_header(&encoder, &map_encoder, d->header, BOREALIS_RECORDING_STATUS_MSG_NUM_FIELDS)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_uint8(&map_encoder, "flags", d->flags)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_string(&map_encoder, "filename", d->filename, d->filename_length)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_float(&map_encoder, "seconds_written", d->seconds_written)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encode_key_value_float(&map_encoder, "seconds_free", d->seconds_free)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = encoder_message_finish(&encoder, &map_encoder)) != CborNoError && err != CborErrorOutOfMemory) break;

        if (err != CborNoError) break;

        /* no errors have occurred, return normally */
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
        return CborNoError;
    } while (0);

    encoder_message_check_memory(&encoder, err);

    return err;
}

CborError borealis_spectrum_data_decode(struct borealis_spectrum_data * d, uint8_t *cbor_buffer, size_t size) {
    CborValue map;
    CborParser parser;
    CborValue value;

    d->spectrum_as_base64 = NULL;

    CborError err;
    do {
        err = decoder_message_enter(&map, &value, &parser, cbor_buffer, size, BOREALIS_SPECTRUM_MSG_NUM_FIELDS);

        if ((err = SensorHeaderMsg::decode(value, d->header)) != CborNoError) break;

        if (CborNoError != decode_key_value_float(&d->dt, &value, "dt")) break;
        if (CborNoError != decode_key_value_float(&d->df, &value, "df")) break;
        if (CborNoError != decode_key_value_uint8(&d->bands_per_octave, &value, "df")) break;
        if (CborNoError != decode_key_value_string(&d->spectrum_as_base64, &d->spectrum_length, &value, "spectrum_as_base64")) break;

        if (err != CborNoError) break;

        err = decoder_message_leave(&value, &map);

        return err;
    } while (0);

    /* we get here only on error. free the allocation if there was one */
#ifndef CI_TEST
    bm_free(d->spectrum_as_base64);
#else
    free(d->spectrum_as_base64);
#endif
    d->spectrum_as_base64 = NULL;

    return err;
}

CborError borealis_levels_decode(struct borealis_levels * d, uint8_t *cbor_buffer, size_t size) {
    CborValue map;
    CborParser parser;
    CborValue value;

    d->levels_as_base64 = NULL;

    CborError err;
    do {
        err = decoder_message_enter(&map, &value, &parser, cbor_buffer, size, BOREALIS_LEVELS_MSG_NUM_FIELDS);

        // header
        if ((err = SensorHeaderMsg::decode(value, d->header)) != CborNoError) break;

        if (CborNoError != decode_key_value_float(&d->dt, &value, "dt")) break;
        if (CborNoError != decode_key_value_float(&d->dt_report, &value, "dt_report")) break;
        if (CborNoError != decode_key_value_uint8(&d->first_band_index, &value, "first_band_index")) break;
        if (CborNoError != decode_key_value_string(&d->levels_as_base64, &d->levels_length, &value, "spectrum_as_base64")) break;

        if (err != CborNoError) break;

        err = decoder_message_leave(&value, &map);

        return err;
    } while (0);

    /* we get here only on error. free the allocation if there was one */
#ifndef CI_TEST
    bm_free(d->levels_as_base64);
#else
    free(d->levels_as_base64);
#endif
    d->levels_as_base64 = NULL;

    return err;
}

CborError borealis_recording_status_decode(struct borealis_recording_status * d, uint8_t *cbor_buffer, size_t size) {
    CborValue map;
    CborParser parser;
    CborValue value;

    d->filename = NULL;

    CborError err;
    do {
        err = decoder_message_enter(&map, &value, &parser, cbor_buffer, size, BOREALIS_RECORDING_STATUS_MSG_NUM_FIELDS);

        if ((err = SensorHeaderMsg::decode(value, d->header)) != CborNoError) break;

        if (CborNoError != decode_key_value_uint8(&d->flags, &value, "flags")) break;
        if (CborNoError != decode_key_value_string(&d->filename, &d->filename_length, &value, "filename")) break;
        if (CborNoError != decode_key_value_float(&d->seconds_written, &value, "seconds_written")) break;
        if (CborNoError != decode_key_value_float(&d->seconds_free, &value, "seconds_free")) break;

        if (err != CborNoError) break;

        err = decoder_message_leave(&value, &map);

        return err;
    } while (0);

    /* we get here only on error. free the allocation if there was one */
#ifndef CI_TEST
    bm_free(d->filename);
#else
    free(d->filename);
#endif
    d->filename = NULL;

    return err;
}
