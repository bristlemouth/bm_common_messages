#pragma once
#include <stdint.h>
#include "cbor.h"
#include "sensor_header_msg.h"

#ifdef __cplusplus
namespace AanderaaCurrentMeterDataMsg {

constexpr size_t NUM_FIELDS = 15;

struct Data {
  sensor_header_t header;
  double abs_speed_cm_s;
  double direction_deg_m;
  double north_cm_s;
  double east_cm_s;
  double heading_deg_m;
  double tilt_x_deg;
  double tilt_y_deg;
  double single_ping_std_cm_s;
  double transducer_strength_db;
  double ping_count;
  double abs_tilt_deg;
  double max_tilt_deg;
  double std_tilt_deg;
  double temperature_deg_c;
};

CborError encode(CborEncoder &map_encoder, Data &d);

CborError decode(CborValue &map, Data &d);

} // namespace AanderaaCurrentMeterDataMsg

extern "C" {
#endif

CborError aanderaa_current_meter_data_encode(CborEncoder * const map_encoder,
                               const sensor_header_t header,
                               const double abs_speed_cm_s,
                               const double direction_deg_m,
                               const double north_cm_s,
                               const double east_cm_s,
                               const double heading_deg_m,
                               const double tilt_x_deg,
                               const double tilt_y_deg,
                               const double single_ping_std_cm_s,
                               const double transducer_strength_db,
                               const double ping_count,
                               const double abs_tilt_deg,
                               const double max_tilt_deg,
                               const double std_tilt_deg,
                               const double temperature_deg_c);

CborError aanderaa_current_meter_data_decode(                               sensor_header_t * const header_p,
                               double * const abs_speed_cm_s_p,
                               double * const direction_deg_m_p,
                               double * const north_cm_s_p,
                               double * const east_cm_s_p,
                               double * const heading_deg_m_p,
                               double * const tilt_x_deg_p,
                               double * const tilt_y_deg_p,
                               double * const single_ping_std_cm_s_p,
                               double * const transducer_strength_db_p,
                               double * const ping_count_p,
                               double * const abs_tilt_deg_p,
                               double * const max_tilt_deg_p,
                               double * const std_tilt_deg_p,
                               double * const temperature_deg_c_p,
                               CborValue * const map);

#ifdef __cplusplus
}
#endif
