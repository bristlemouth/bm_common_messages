#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace BmSoftDataMsg {

constexpr size_t NUM_FIELDS = 2;

struct Data {
  sensor_header_t header;
  double temperature_deg_c;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace BmSoftDataMsg

extern "C" {
#endif

CborError bm_soft_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double temperature_deg_c);

CborError bm_soft_data_decode(                               sensor_header_t * const header_p,
                               double * const temperature_deg_c_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
