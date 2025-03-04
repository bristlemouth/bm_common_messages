#pragma once
#include "cbor.h"
#ifdef __cplusplus
#include "sensor_header_msg.h"
#endif

// For Applied Ocean Sciences Acoustic Recorder: BOREALIS
// Bristlemouth Open Recorder & Edge-processor Acoustic Low-power Information System

#define BOREALIS_SPECTRUM_MSG_VERSION 1
#define BOREALIS_SPECTRUM_MSG_NUM_FIELDS (4 + SensorHeaderMsg::NUM_FIELDS)

/* hopefully temporary c-accessible shim for things defined in a vestigial c++ namespaced struct */
struct sensor_header_msg_data {
    uint32_t version;
    uint64_t reading_time_utc_ms;
    uint64_t reading_uptime_millis;
    uint64_t sensor_reading_time_ms;
};

#ifdef __cplusplus
/* validate that the c accessible and c++ namespace definitions of these struct members are compatible */
static_assert(offsetof(struct sensor_header_msg_data, version) == offsetof(SensorHeaderMsg::Data, version) &&
              offsetof(struct sensor_header_msg_data, reading_time_utc_ms) == offsetof(SensorHeaderMsg::Data, reading_time_utc_ms) &&
              offsetof(struct sensor_header_msg_data, reading_uptime_millis) == offsetof(SensorHeaderMsg::Data, reading_uptime_millis) &&
              offsetof(struct sensor_header_msg_data, sensor_reading_time_ms) == offsetof(SensorHeaderMsg::Data, sensor_reading_time_ms) &&
              sizeof(struct sensor_header_msg_data) == sizeof(SensorHeaderMsg::Data),
              "struct definition disagreement");
#endif

struct borealis_spectrum_data {
#ifdef __cplusplus
    SensorHeaderMsg::Data header;
#else
    struct sensor_header_msg_data header;
#endif

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
#ifdef __cplusplus
    SensorHeaderMsg::Data header;
#else
    struct sensor_header_msg_data header;
#endif
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
#ifdef __cplusplus
    SensorHeaderMsg::Data header;
#else
    struct sensor_header_msg_data header;
#endif

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
