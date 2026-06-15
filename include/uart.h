#ifndef UART_H
#define UART_H

#include <stdint.h>
#define RESETS_BASE   0x4000c000
#define RESETS_RESET ((volatile uint32_t *)(RESETS_BASE + 0x0))
#define RESETS_RESET_DONE ((volatile uint32_t *)RESETS_BASE + 0x8)
#define RESET_UART0_BIT (1 << 22)
#define UART0 ((uart_registers *)0x40034000)
#define UART1 ((uart_registers *)0x40038000)
typedef struct __attribute__((packed)) {
    volatile uint32_t DR;
    volatile uint32_t RSRECR;
    uint32_t _reserved0[4];
    const volatile uint32_t FR;
    uint32_t _reserved1;
    volatile uint32_t ILPR;
    volatile uint32_t IBRD;
    volatile uint32_t FBRD;
    volatile uint32_t LCRH;
    volatile uint32_t CR;
} uart_registers;
void uart_init(uint32_t baud);
void uart_putchar(char c);
void uart_getc(char* c);
void uart_write(const char* data);

// static uart_registers* uart0 = (uart_registers*)0x40034000;
// static uart_registers* uart1 = (uart_registers*)0x40038000;  static creates copies in all c files which means 2 variables in memory pointing to the same address wastes RAM and memory

#endif