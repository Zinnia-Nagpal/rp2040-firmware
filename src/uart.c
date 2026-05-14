#include "uart.h"

 #define uart_clock_get_hz 125000000

typedef enum{
    UART_OK = 0,
    UART_INVALID_ARGUMENT_BAUDRATE,
    UART_INVALID_ARGUMENT_WORDSIZE,
    UART_INVALID_ARGUMENT_STOP_BITS,
   UART_RECEIVE_ERROR,
   UART_NO_DATA,
} uart_error;
void uart_init(uint32_t baud){
   /* Disable the uart*/
   UART0 -> CR &= ~(1 << 0);
   while(UART0 -> FR & (1 << 3));    /* wait for transmiision to finish*/
   UART0 -> LCRH &= ~(1 << 4);      /* flush the FIFO  On many UART controllers, disabling the FIFOs effectively flushes them. By toggling this bit off, any remaining data sitting in the hardware transmit or receive buffers is discarded */
  UART0 -> IBRD = uart_clock_get_hz / (16 * baud);
  UART0 -> FBRD = ((uart_clock_get_hz % (16 * baud))* 64) / (16 * baud );
  UART0 -> LCRH =  (1 << 4) | (1 << 5)|(1 << 6);
  UART0 -> CR = (1 << 0)| (1 << 9) | (1 << 8);
}
 void uart_putchar(char c){
      while(UART0 -> FR & (1 << 5));  /* bit 5 = TXFF (TX FIFO full) - correct */
      UART0 -> DR = c;
   }

   void uart_getc(char* c){
       while(UART0->FR & (1 << 4));  /* wait while RX FIFO empty */
    *c = UART0->DR;
    }
  void uart_write(const char* data){
   while(*data){
      uart_putchar(*data++);
   }
}

