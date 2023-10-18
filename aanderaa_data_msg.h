#pragma once
#include "cbor.h"

namespace AanderaaDataMsg {

constexpr size_t NUM_FIELDS = 5;

struct Data {
    double abs_speed_cm_s;
    double direction_deg_m;
    double north_cm_s;
    double east_cm_s;
    double heading_deg_m;
    double tilt_x_deg;
    double tilt_y_deg;
    double sp_std_cm_s;
    double strength_db;
    double ping_count;
    double abs_tilt_deg;
    double max_tilt_deg;
    double std_tilt_deg;
    double temperature_deg_c;
};

CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                 size_t *encoded_len);

CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

} // namespace AanderaaDataMsg