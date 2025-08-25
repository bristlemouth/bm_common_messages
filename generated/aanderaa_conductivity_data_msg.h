#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace AanderaaConductivityDataMsg {

constexpr size_t NUM_FIELDS = 7;

struct Data {
  sensor_header_t header;
  double conductivity_ms_cm;
  double temperature_deg_c;
  double salinity_psu;
  double water_density_kg_m3;
  double sound_speed_m_s;
  float depth_m;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace AanderaaConductivityDataMsg

extern "C" {
#endif

CborError aanderaa_conductivity_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double conductivity_ms_cm,
                               const double temperature_deg_c,
                               const double salinity_psu,
                               const double water_density_kg_m3,
                               const double sound_speed_m_s,
                               const float depth_m);

CborError aanderaa_conductivity_data_decode(                               sensor_header_t * const header_p,
                               double * const conductivity_ms_cm_p,
                               double * const temperature_deg_c_p,
                               double * const salinity_psu_p,
                               double * const water_density_kg_m3_p,
                               double * const sound_speed_m_s_p,
                               float * const depth_m_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
