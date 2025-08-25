#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace PmeWipeMsg {

constexpr size_t NUM_FIELDS = 3;

struct Data {
  sensor_header_t header;
  double wipe_current_mean_ma;
  double wipe_duration_s;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace PmeWipeMsg

extern "C" {
#endif

CborError pme_wipe_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double wipe_current_mean_ma,
                               const double wipe_duration_s);

CborError pme_wipe_decode(                               sensor_header_t * const header_p,
                               double * const wipe_current_mean_ma_p,
                               double * const wipe_duration_s_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
