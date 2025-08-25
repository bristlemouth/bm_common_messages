#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace BmTurbidityDataMsg {

constexpr size_t NUM_FIELDS = 3;

struct Data {
  sensor_header_t header;
  double s_signal;
  double r_signal;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace BmTurbidityDataMsg

extern "C" {
#endif

CborError bm_turbidity_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double s_signal,
                               const double r_signal);

CborError bm_turbidity_data_decode(                               sensor_header_t * const header_p,
                               double * const s_signal_p,
                               double * const r_signal_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
