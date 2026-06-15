#include <stdint.h>

#ifndef TELEMETRY_H
#define TELEMETRY_H


#define PKT_SOF              0xAA
#define PKT_TYPE_TEMPERATURE 0x01
#define PKT_TYPE_STATUS      0x02
#define PKT_MAX_PAYLOAD      32

typedef struct  {
   uint8_t SOF;
   uint8_t TYPE;
   uint8_t LENGTH;
   uint8_t PAYLOAD[32];
   uint16_t crc16;
} telemetry_packet_t;

void telemtry_pack(telemetry_packet_t *pkt, uint8_t type, uint8_t *payload, uint8_t length);
uint8_t verify_pack(const telemetry_packet_t *pkt);



#endif