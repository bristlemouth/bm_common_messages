#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace BmRbrDataMsg {

constexpr size_t NUM_FIELDS = 4;

struct Data {
  sensor_header_t header;
  uint32_t sensor_type;
  double temperature_deg_c;
  double pressure_deci_bar;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace BmRbrDataMsg

extern "C" {
#endif

CborError bm_rbr_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const uint32_t sensor_type,
                               const double temperature_deg_c,
                               const double pressure_deci_bar);

CborError bm_rbr_data_decode(                               sensor_header_t * const header_p,
                               uint32_t * const sensor_type_p,
                               double * const temperature_deg_c_p,
                               double * const pressure_deci_bar_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
