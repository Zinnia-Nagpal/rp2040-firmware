#include "crc16.h"

static uint16_t crc16_table[256];
static int table_ready = 0;

static void build_table(void) {
    for (int i = 0; i < 256; i++) {
        uint16_t crc = i << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
        crc16_table[i] = crc;
    }
    table_ready = 1;
}

uint16_t crc16_compute(const uint8_t *buffer, uint32_t length) {
    if (!table_ready) build_table();
    uint16_t crc = 0xFFFF;
    while (length--) {
        crc = (crc << 8) ^ crc16_table[((crc >> 8) ^ *buffer++) & 0xFF];
    }
    return crc;
}

uint8_t crc16_verify(const uint8_t *buffer, uint32_t length, uint16_t expected) {
    return crc16_compute(buffer, length) == expected;
}