#include "uart.h"
int main(void) {
    uart_init(115200);
    uart_write("Hello from RP2040\n");
    while(1);
    return 0;
}

