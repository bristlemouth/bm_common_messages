#pragma once
#include "cbor.h"

#ifdef __cplusplus
extern "C" {
#endif

#define METRICS_REPLY_VERSION 1
#define METRICS_MAX_PORTS 2          /* ADIN2111 has 2 ports */
#define METRICS_REPLY_NUM_FIELDS 4   /* v, node, up, p */
#define METRICS_PORT_NUM_FIELDS 5    /* mse, sqi, lq, rxe, sye */

typedef struct {
  uint16_t mse_val;          /* raw MSE_VAL register */
  uint8_t  sqi;              /* signal quality indicator */
  uint8_t  link_quality;     /* adi_phy_LinkQuality_e: 0 poor / 1 marginal / 2 good */
  uint16_t rx_err_count;     /* frame_check_rx_error_count */
  uint16_t symbol_err_count; /* frame_check_error_counters.SYMB_ERR_CNT */
} MetricsPortStats;

typedef struct {
  uint8_t  version;
  uint64_t node_id;
  uint32_t uptime_ms;
  uint8_t  num_ports;
  MetricsPortStats ports[METRICS_MAX_PORTS];
} MetricsReplyData;

CborError metrics_reply_encode(const MetricsReplyData *d, uint8_t *cbor_buffer,
                               size_t size, size_t *encoded_len);

#ifdef __cplusplus
}
#endif
