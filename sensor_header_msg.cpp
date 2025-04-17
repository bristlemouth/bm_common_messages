#include "sensor_header_msg.h"
#include "bm_config.h"

CborError SensorHeaderMsg::encode(CborEncoder &map_encoder, Data &d) {
    return sensor_header_encode(&map_encoder, d.version, d.reading_time_utc_ms, d.reading_uptime_millis, d.sensor_reading_time_ms);
}

CborError SensorHeaderMsg::decode(CborValue &map, Data &d) {
    return sensor_header_decode(&d.version, &d.reading_time_utc_ms, &d.reading_uptime_millis, &d.sensor_reading_time_ms, &map);
}
