#include "telemetry.h"
#include "crc16.h"
#include <string.h> 
void telemetry_pack(telemetry_packet_t *pkt, uint8_t type, uint8_t *payload, uint8_t length)
{
    pkt->SOF = PKT_SOF;
    pkt->TYPE = type;
    pkt-> LENGTH = length;
    memcpy(pkt -> PAYLOAD , payload, length);
    pkt -> crc16 = crc16_compute((uint8_t*) pkt, 3 + length);
}
 
uint8_t verify_pack(const telemetry_packet_t *pkt){
     return crc16_verify((uint8_t*) pkt, 3 + pkt -> LENGTH, pkt -> crc16);
}