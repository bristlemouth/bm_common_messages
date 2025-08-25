#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace BarometricPressureDataMsg {

constexpr size_t NUM_FIELDS = 2;

struct Data {
  sensor_header_t header;
  double barometric_pressure_mbar;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace BarometricPressureDataMsg

extern "C" {
#endif

CborError barometric_pressure_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double barometric_pressure_mbar);

CborError barometric_pressure_data_decode(                               sensor_header_t * const header_p,
                               double * const barometric_pressure_mbar_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
