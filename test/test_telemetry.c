#include "telemetry.h"
#include <stdio.h>
#include <stdint.h>

// Create a packet
// Pack it
// Verify it
// Print pass/fail
int main(){
  uint8_t payload[] = {0x01, 0x02, 0x03, 0x04};
     telemetry_packet_t pkt;

     telemetry_pack(&pkt, PKT_TYPE_TEMPERATURE, payload , 4);
     /* verify it */
    if (verify_pack(&pkt)) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}