
#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>

uint16_t crc16_compute(const uint8_t *buffer, uint32_t length);
uint8_t  crc16_verify(const uint8_t *buffer, uint32_t length, uint16_t expected);

#endif
