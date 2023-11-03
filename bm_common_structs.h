#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////
/* CONFIGURATION*/
/////////////////////////////
typedef enum {
  BM_COMMON_CFG_PARTITION_USER,
  BM_COMMON_CFG_PARTITION_SYSTEM,
  BM_COMMON_CFG_PARTITION_HARDWARE,
} bm_common_config_partition_e;

typedef struct {
  // Node ID of the target node for which the request is being made.
  uint64_t target_node_id;
  // Node ID of the source node
  uint64_t source_node_id;
  // message payload
  uint8_t payload[0];
} __attribute__((packed)) bm_common_config_header_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
  // String length of the key (without terminator)
  uint8_t key_length;
  // Key string
  char key[0];
} __attribute__((packed)) bm_common_config_get_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
  // Length of cbor buffer 
  uint32_t data_length;
  // cbor buffer
  uint8_t data[0];
} __attribute__((packed)) bm_common_config_value_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
  // String length of the key (without terminator)
  uint8_t key_length;
  // Length of cbor encoded data buffer
  uint32_t data_length;
  // cbor encoded data
  uint8_t keyAndData[0];
} __attribute__((packed)) bm_common_config_set_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
} __attribute__((packed)) bm_common_config_commit_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
} __attribute__((packed)) bm_common_config_status_request_t;

typedef struct {
  // String length of the key (without terminator)
  uint8_t key_length;
  // Key string
  char key[0];
} __attribute__((packed)) bm_common_config_status_key_data_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
  // True if there are changes to be committed, false otherwise.
  bool committed;
  // Number of keys
  uint8_t num_keys;
  // Key Data
  uint8_t keyData[0];
} __attribute__((packed)) bm_common_config_status_response_t;

typedef struct {
  bm_common_config_header_t header;
  // Partition id
  bm_common_config_partition_e partition;
  // String length of the key (without terminator)
  uint8_t key_length;
  // Key string
  char key[0];
} __attribute__((packed)) bm_common_config_delete_key_request_t;

typedef struct {
  bm_common_config_header_t header;
  // success
  bool success;
  // Partition id
  bm_common_config_partition_e partition;
  // String length of the key (without terminator)
  uint8_t key_length;
  // Key string
  char key[0];
} __attribute__((packed)) bm_common_config_delete_key_response_t;


typedef enum {
    BM_COMMON_WIRELESS_NETWORK_TYPE_CELLULAR_IRI_FALLBACK = (1 << 0),
    BM_COMMON_WIRELESS_NETWORK_TYPE_CELLULAR_ONLY = (1 << 1),
} bm_common_wireless_network_type_e;

typedef struct {
    // Wireless network type to send over.
    bm_common_wireless_network_type_e type;
    // Data
    uint8_t data[0];
} __attribute__((packed)) bm_common_wireless_network_data_header_t;

typedef struct {
  // Partition id
  bm_common_config_partition_e partition;
  // Partion crc
  uint32_t crc32;
} __attribute__((packed)) bm_common_config_crc_t;

typedef struct {
  // fw version
  uint8_t major;
  uint8_t minor;
  uint8_t revision;
  uint32_t gitSHA;
} __attribute__((packed)) bm_common_fw_version_t;

typedef struct {
  // Number of Nodes in the list
  uint16_t num_nodes;
  // List of nodes
  uint64_t list[0];
} __attribute__((packed)) bm_common_topology_node_list_t;

typedef struct {
  // Size of the map in bytes
  uint16_t map_size_bytes;
  // Cbor encoded network configuration map
  uint8_t map[0];
} __attribute__((packed)) bm_common_network_configuration_map_t;

typedef struct {
  // crc of the this message (excluding itself)
  uint32_t network_crc32;
  // Config crc
  bm_common_config_crc_t config_crc;
  // fw info
  bm_common_fw_version_t fw_info;
  // topology list
  bm_common_topology_node_list_t node_list;
  // configuration map
  bm_common_network_configuration_map_t config_map;
} __attribute__((packed)) bm_common_network_info_t;

#ifdef __cplusplus
}
#endif
