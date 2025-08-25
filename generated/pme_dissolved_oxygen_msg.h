#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace PmeDissolvedOxygenMsg {

constexpr size_t NUM_FIELDS = 6;

struct Data {
  sensor_header_t header;
  double temperature_deg_c;
  double do_mg_per_l;
  double quality;
  double do_saturation_pct;
  float salinity_ppt;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace PmeDissolvedOxygenMsg

extern "C" {
#endif

CborError pme_dissolved_oxygen_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double temperature_deg_c,
                               const double do_mg_per_l,
                               const double quality,
                               const double do_saturation_pct,
                               const float salinity_ppt);

CborError pme_dissolved_oxygen_decode(                               sensor_header_t * const header_p,
                               double * const temperature_deg_c_p,
                               double * const do_mg_per_l_p,
                               double * const quality_p,
                               double * const do_saturation_pct_p,
                               float * const salinity_ppt_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
