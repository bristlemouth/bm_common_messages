#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

// For Applied Ocean Sciences Acoustic Recorder: BOREALIS
// Bristlemouth Open Recorder & Edge-processor Acoustic Low-power Information System

#define BOREALIS_SPECTRUM_MSG_VERSION 1
#define BOREALIS_SPECTRUM_MSG_NUM_FIELDS (4 + SensorHeaderMsg::NUM_FIELDS)

struct borealis_spectrum_data {
    SensorHeaderMsg::Data header;
    float dt;
    float df;
    uint8_t bands_per_octave;
    //on the encode side this points into a line buffer and is the given length, and is not null terminated
    //on the decode side, this will be bm_malloc'd inside the decode function and must be bm_freed by the caller
    char * spectrum_as_base64;
    size_t spectrum_length;//this does not get tx'd as its own cbor field
};

#define BOREALIS_LEVELS_MSG_VERSION 1
#define BOREALIS_LEVELS_MSG_NUM_FIELDS (4 + SensorHeaderMsg::NUM_FIELDS)

struct borealis_levels {
    SensorHeaderMsg::Data header;
    float dt;
    float dt_report;
    uint8_t first_band_index;
    //on the encode side this points into a line buffer and is the given length, and is not null terminated
    //on the decode side, this will be bm_malloc'd inside the decode function and must be bm_freed by the caller
    char * levels_as_base64;
    size_t levels_length;//this does not get tx'd as its own cbor field
};

#define BOREALIS_RECORDING_STATUS_MSG_VERSION 1
#define BOREALIS_RECORDING_STATUS_MSG_NUM_FIELDS (4 + SensorHeaderMsg::NUM_FIELDS)

struct borealis_recording_status {
    SensorHeaderMsg::Data header;

    uint8_t flags; /* least significant bit indicates whether recording or not */
    char * filename;
    size_t filename_length; /* not transmitted separately */
    float seconds_written;
    float seconds_free;
};

#ifdef __cplusplus
extern "C"
{
#endif
CborError borealis_spectrum_data_encode(struct borealis_spectrum_data * d, uint8_t * cbor_buffer, size_t size,
                                        size_t * encoded_len);

CborError borealis_spectrum_data_decode(struct borealis_spectrum_data * d, uint8_t * cbor_buffer, size_t size);

CborError borealis_levels_encode(struct borealis_levels * d, uint8_t * cbor_buffer, size_t size,
                                        size_t * encoded_len);

CborError borealis_levels_decode(struct borealis_levels * d, uint8_t * cbor_buffer, size_t size);

CborError borealis_recording_status_encode(struct borealis_recording_status * d, uint8_t * cbor_buffer, size_t size,
                                           size_t * encoded_len);
CborError borealis_recording_status_decode(struct borealis_spectrum_data * d, uint8_t * cbor_buffer, size_t size);

#ifdef __cplusplus
}
#endif
