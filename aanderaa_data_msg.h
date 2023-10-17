#pragma once
#include "cbor.h"

namespace AanderaaDataMsg {

constexpr size_t NUM_FIELDS = 5;

struct Data {
    double abs_speed_mean_cm_s;
    double abs_speed_stddev_cm_s;
    double direction_circ_mean_rad;
    double direction_circ_stddev_rad;
    double temperature_mean_degC;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace AanderaaDataMsg