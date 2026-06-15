#include <stdio.h>
#include <stdint.h>
#include "crc16.h"

int main() {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    uint16_t crc = crc16_compute(data, 4);
    printf("CRC16: 0x%04X\n", crc);
    
    if (crc16_verify(data, 4, crc)) {
        printf("PASS: verify works\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}