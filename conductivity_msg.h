#pragma once
#include "cbor.h"
#include "sensor_header_msg.h"

// For the Aanderaa 5990 Conductivity Sensor

namespace AanderaaConductivityMsg {

    constexpr uint32_t VERSION = 1;
    constexpr size_t NUM_FIELDS = 5 + SensorHeaderMsg::NUM_FIELDS;

    struct Data {
        SensorHeaderMsg::Data header;
        double conductivity_ms_cm;
        double temperature_deg_c;
        double salinity_psu;
        double water_density_kg_m3;
        double sound_speed_m_s;
    };

    CborError encode(Data &d, uint8_t *cbor_buffer, size_t size,
                     size_t *encoded_len);

    CborError decode(Data &d, const uint8_t *cbor_buffer, size_t size);

}