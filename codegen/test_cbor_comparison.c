#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "cbor.h"

// Include both versions - we'll need to rename one to avoid conflicts
#include "../sensor_header_msg.h"

// For the generated version, we'll declare the functions manually to avoid naming conflicts
CborError sensor_header_encode_generated(CborEncoder * const map_encoder,
                                         const uint32_t version,
                                         const uint64_t reading_time_utc_ms,
                                         const uint64_t reading_uptime_millis,
                                         const uint64_t sensor_reading_time_ms);

void print_hex(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (len % 16 != 0) printf("\n");
}

int main() {
    uint8_t buffer_handwritten[256];
    uint8_t buffer_generated[256];

    CborEncoder encoder_hw, map_encoder_hw;
    CborEncoder encoder_gen, map_encoder_gen;

    // Test data
    uint32_t version = 1;
    uint64_t reading_time_utc_ms = 1634567890123ULL;
    uint64_t reading_uptime_millis = 987654321ULL;
    uint64_t sensor_reading_time_ms = 1634567890456ULL;

    // Encode with handwritten version
    cbor_encoder_init(&encoder_hw, buffer_handwritten, sizeof(buffer_handwritten), 0);
    cbor_encoder_create_map(&encoder_hw, &map_encoder_hw, 4);
    CborError err_hw = sensor_header_encode(&map_encoder_hw, version, reading_time_utc_ms,
                                           reading_uptime_millis, sensor_reading_time_ms);
    cbor_encoder_close_container(&encoder_hw, &map_encoder_hw);
    size_t len_hw = cbor_encoder_get_buffer_size(&encoder_hw, buffer_handwritten);

    // Encode with generated version
    cbor_encoder_init(&encoder_gen, buffer_generated, sizeof(buffer_generated), 0);
    cbor_encoder_create_map(&encoder_gen, &map_encoder_gen, 4);
    CborError err_gen = sensor_header_encode_generated(&map_encoder_gen, version, reading_time_utc_ms,
                                                      reading_uptime_millis, sensor_reading_time_ms);
    cbor_encoder_close_container(&encoder_gen, &map_encoder_gen);
    size_t len_gen = cbor_encoder_get_buffer_size(&encoder_gen, buffer_generated);

    printf("Handwritten version (error: %d, length: %zu):\n", err_hw, len_hw);
    print_hex(buffer_handwritten, len_hw);

    printf("\nGenerated version (error: %d, length: %zu):\n", err_gen, len_gen);
    print_hex(buffer_generated, len_gen);

    printf("\nBinary identical: %s\n",
           (len_hw == len_gen && memcmp(buffer_handwritten, buffer_generated, len_hw) == 0) ? "YES" : "NO");

    return 0;
}