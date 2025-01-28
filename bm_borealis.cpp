#include "bm_borealis.h"
#include "bm_config.h"
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

static CborError send_key_value_float(CborEncoder * map_encoder, const char * name, const float value) {
    CborError err;
    if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
        debug_printf("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n", __func__, name, err);
        if (err != CborErrorOutOfMemory) return err;
    }

    if ((err = cbor_encode_float(map_encoder, value)) != CborNoError)
        debug_printf("error: %s(%s): cbor_encode_float() failed: %d\r\n", __func__, name, err);

    return err;
}

static CborError send_key_value_uint8(CborEncoder * map_encoder, const char * name, const float value) {
    CborError err;
    if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
        debug_printf("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n", __func__, name, err);
        if (err != CborErrorOutOfMemory) return err;
    }

    if ((err = cbor_encode_simple_value(map_encoder, value)) != CborNoError)
        debug_printf("error: %s(%s): cbor_encode_simple_value() failed: %d\r\n", __func__, name, err);
    return err;
}

static CborError send_key_value_string(CborEncoder * map_encoder, const char * name, const char * value, const size_t len) {
    CborError err;
    if ((err = cbor_encode_text_stringz(map_encoder, name)) != CborNoError) {
        debug_printf("error: %s(%s): cbor_encode_text_stringz() failed: %d\r\n", __func__, name, err);
        if (err != CborErrorOutOfMemory) return err;
    }

    if ((err = cbor_encode_text_string(map_encoder, value, len)) != CborNoError)
        debug_printf("error: %s(%s): cbor_encode_byte_string() failed: %d\r\n", __func__, name, err);
    return err;
}

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

static CborError finish_message(CborEncoder * encoder, CborEncoder * map_encoder) {
    const CborError err = cbor_encoder_close_container(encoder, map_encoder);
    if (err != CborNoError)
        debug_printf("error: %s: cbor_encoder_close_container failed: %d\r\n", __func__, err);
    return err;
}

CborError borealis_spectrum_data_encode(struct borealis_spectrum_data * d, uint8_t * cbor_buffer, size_t size, size_t * encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder;
    cbor_encoder_init(&encoder, cbor_buffer, size, 0);

    do {
        if ((err = create_map_and_send_header(&encoder, &map_encoder, d->header, BOREALIS_SPECTRUM_MSG_NUM_FIELDS)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_float(&map_encoder, "dt", d->dt)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_float(&map_encoder, "df", d->df)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_uint8(&map_encoder, "bands_per_octave", d->bands_per_octave)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_string(&map_encoder, "spectrum", d->spectrum_as_base64, d->spectrum_length)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = finish_message(&encoder, &map_encoder)) != CborNoError && err != CborErrorOutOfMemory) break;

        if (err != CborNoError) break;

        /* no errors have occurred, return normally */
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
        return CborNoError;
    } while (0);

    /* we get here due to any error */
    if (CborErrorOutOfMemory == err)
        debug_printf("error: %s: extra_bytes_needed: %zu\r\n", __func__, cbor_encoder_get_extra_bytes_needed(&encoder));

    return err;
}

CborError borealis_levels_encode(struct borealis_levels * d, uint8_t * cbor_buffer, size_t size, size_t * encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder;
    cbor_encoder_init(&encoder, cbor_buffer, size, 0);

    do {
        if ((err = create_map_and_send_header(&encoder, &map_encoder, d->header, BOREALIS_LEVELS_MSG_NUM_FIELDS)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_float(&map_encoder, "dt", d->dt)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_float(&map_encoder, "dt_report", d->dt_report)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_uint8(&map_encoder, "first_band_index", d->first_band_index)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_string(&map_encoder, "levels", d->levels_as_base64, d->levels_length)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = finish_message(&encoder, &map_encoder)) != CborNoError && err != CborErrorOutOfMemory) break;

        if (err != CborNoError) break;

        /* no errors have occurred, return normally */
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
        return CborNoError;
    } while (0);

    /* we get here due to any error */
    if (CborErrorOutOfMemory == err)
        debug_printf("error: %s: extra_bytes_needed: %zu\r\n", __func__, cbor_encoder_get_extra_bytes_needed(&encoder));

    return err;
}

CborError borealis_recording_status_encode(struct borealis_recording_status * d, uint8_t * cbor_buffer, size_t size, size_t * encoded_len) {
    CborError err;
    CborEncoder encoder, map_encoder;
    cbor_encoder_init(&encoder, cbor_buffer, size, 0);

    do {
        if ((err = create_map_and_send_header(&encoder, &map_encoder, d->header, BOREALIS_RECORDING_STATUS_MSG_NUM_FIELDS)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_uint8(&map_encoder, "flags", d->flags)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_string(&map_encoder, "filename", d->filename, d->filename_length)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_float(&map_encoder, "seconds_written", d->seconds_written)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = send_key_value_float(&map_encoder, "seconds_free", d->seconds_free)) != CborNoError && err != CborErrorOutOfMemory) break;
        if ((err = finish_message(&encoder, &map_encoder)) != CborNoError && err != CborErrorOutOfMemory) break;

        if (err != CborNoError) break;

        /* no errors have occurred, return normally */
        *encoded_len = cbor_encoder_get_buffer_size(&encoder, cbor_buffer);
        return CborNoError;
    } while (0);

    /* we get here due to any error */
    if (CborErrorOutOfMemory == err)
        debug_printf("error: %s: extra_bytes_needed: %zu\r\n", __func__, cbor_encoder_get_extra_bytes_needed(&encoder));

    return err;
}

static int get_key_value_float(float * out, CborValue * value, const char * key_expected) {
    CborError err;

    if (!cbor_value_is_text_string(value)) {
        err = CborErrorIllegalType;
        debug_printf("error: %s(%s): expected string key but got something else\r\n", __func__, key_expected);
        return -1;
    }

    if ((err = cbor_value_advance(value)) != CborNoError) return -1;
    if ((err = cbor_value_get_float(value, out)) != CborNoError) return -1;
    if ((err = cbor_value_advance(value)) != CborNoError) return -1;

    return 0;
}

static int get_key_value_uint8(uint8_t * out, CborValue * value, const char * key_expected) {
    CborError err;

    if (!cbor_value_is_text_string(value)) {
        err = CborErrorIllegalType;
        debug_printf("error: %s(%s): expected string key but got something else\r\n", __func__, key_expected);
        return -1;
    }

    if ((err = cbor_value_advance(value)) != CborNoError) return -1;
    if ((err = cbor_value_get_simple_type(value, out)) != CborNoError) return -1;
    if ((err = cbor_value_advance(value)) != CborNoError) return -1;

    return 0;
}

static int get_key_value_string(char ** out, size_t * len, CborValue * value, const char * key_expected) {
    CborError err;
    if (!cbor_value_is_text_string(value)) {
        err = CborErrorIllegalType;
        debug_printf("error: %s(%s): expected string key but got something else\r\n", __func__, key_expected);
        return -1;
    }

    if ((err = cbor_value_advance(value)) != CborNoError) return -1;

    size_t len_without_zeroterm = 0;
    if ((err = cbor_value_calculate_string_length(value, &len_without_zeroterm)) != CborNoError) return -1;

    size_t allocation_size = len_without_zeroterm + 1;
#ifndef CI_TEST
    *out = (char *)bm_malloc(allocation_size);
#else
    *out = (char *)malloc(allocation_size);
#endif
    if (!(*out)) return -1;

    if ((err = cbor_value_copy_text_string(value, *out, &allocation_size, NULL)) != CborNoError) return -1;

    /* explicitly zero terminate the allocation */
    (*out)[len_without_zeroterm] = '\0';
    *len = len_without_zeroterm;

    if ((err = cbor_value_advance(value)) != CborNoError) return -1;

    return 0;
}

CborError borealis_spectrum_data_decode(struct borealis_spectrum_data * d, uint8_t *cbor_buffer, size_t size) {
    CborParser parser;
    CborValue map;

    d->spectrum_as_base64 = NULL;

    CborError err;
    do {
        if ((err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map)) != CborNoError) break;

        if ((err = cbor_value_validate_basic(&map)) != CborNoError) break;

        if (!cbor_value_is_map(&map)) {
            err = CborErrorIllegalType;
            break;
        }

        size_t num_fields;
        if ((err = cbor_value_get_map_length(&map, &num_fields)) != CborNoError) break;

        if (num_fields != BOREALIS_SPECTRUM_MSG_NUM_FIELDS) {
            err = CborErrorUnknownLength;
            debug_printf("error: %s: expected %zu fields but got %zu\r\n", __func__, BOREALIS_SPECTRUM_MSG_NUM_FIELDS, num_fields);
            break;
        }

        CborValue value;
        if ((err = cbor_value_enter_container(&map, &value)) != CborNoError) break;

        if ((err = SensorHeaderMsg::decode(value, d->header)) != CborNoError) break;

        if (-1 == get_key_value_float(&d->dt, &value, "dt")) break;
        if (-1 == get_key_value_float(&d->df, &value, "df")) break;
        if (-1 == get_key_value_uint8(&d->bands_per_octave, &value, "df")) break;
        if (-1 == get_key_value_string(&d->spectrum_as_base64, &d->spectrum_length, &value, "spectrum_as_base64")) break;

        if (err != CborNoError) break;

        if ((err = cbor_value_leave_container(&map, &value)) != CborNoError) break;

        if (!cbor_value_at_end(&map)) {
            err = CborErrorGarbageAtEnd;
            break;
        }

        return CborNoError;
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
    CborParser parser;
    CborValue map;

    d->levels_as_base64 = NULL;

    CborError err;
    do {
        if ((err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map)) != CborNoError) break;
        if ((err = cbor_value_validate_basic(&map)) != CborNoError) break;

        if (!cbor_value_is_map(&map)) {
            err = CborErrorIllegalType;
            break;
        }

        size_t num_fields;
        if ((err = cbor_value_get_map_length(&map, &num_fields)) != CborNoError) break;

        if (num_fields != BOREALIS_LEVELS_MSG_NUM_FIELDS) {
            err = CborErrorUnknownLength;
            debug_printf("error: %s: expected %zu fields but got %zu\r\n", __func__, BOREALIS_LEVELS_MSG_NUM_FIELDS, num_fields);
            break;
        }

        CborValue value;
        if ((err = cbor_value_enter_container(&map, &value)) != CborNoError) break;

        // header
        if ((err = SensorHeaderMsg::decode(value, d->header)) != CborNoError) break;

        if (-1 == get_key_value_float(&d->dt, &value, "dt")) break;
        if (-1 == get_key_value_float(&d->dt_report, &value, "dt_report")) break;
        if (-1 == get_key_value_uint8(&d->first_band_index, &value, "first_band_index")) break;
        if (-1 == get_key_value_string(&d->levels_as_base64, &d->levels_length, &value, "spectrum_as_base64")) break;

        if (err != CborNoError) break;

        if ((err = cbor_value_leave_container(&map, &value)) != CborNoError) break;

        if (!cbor_value_at_end(&map)) {
            err = CborErrorGarbageAtEnd;
            break;
        }

        return CborNoError;
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
    CborParser parser;
    CborValue map;

    d->filename = NULL;

    CborError err;
    do {
        if ((err = cbor_parser_init(cbor_buffer, size, 0, &parser, &map)) != CborNoError) break;

        if ((err = cbor_value_validate_basic(&map)) != CborNoError) break;

        if (!cbor_value_is_map(&map)) {
            err = CborErrorIllegalType;
            break;
        }

        size_t num_fields;
        if ((err = cbor_value_get_map_length(&map, &num_fields)) != CborNoError) break;

        if (num_fields != BOREALIS_LEVELS_MSG_NUM_FIELDS) {
            err = CborErrorUnknownLength;
            debug_printf("error: %s: expected %zu fields but got %zu\r\n", __func__, BOREALIS_RECORDING_STATUS_MSG_NUM_FIELDS, num_fields);
            break;
        }

        CborValue value;
        if ((err = cbor_value_enter_container(&map, &value)) != CborNoError) break;

        if ((err = SensorHeaderMsg::decode(value, d->header)) != CborNoError) break;

        if (-1 == get_key_value_uint8(&d->flags, &value, "flags")) break;
        if (-1 == get_key_value_string(&d->filename, &d->filename_length, &value, "filename")) break;
        if (-1 == get_key_value_float(&d->seconds_written, &value, "seconds_written")) break;
        if (-1 == get_key_value_float(&d->seconds_free, &value, "seconds_free")) break;

        if (err != CborNoError) break;

        if ((err = cbor_value_leave_container(&map, &value)) != CborNoError) break;

        if (!cbor_value_at_end(&map)) {
            err = CborErrorGarbageAtEnd;
            break;
        }

        return CborNoError;
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
