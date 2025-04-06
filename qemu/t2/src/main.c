#include "uart.h"

void main() {
  // UART0_FCR = UARTFCR_FFENA;   // Set the FIFO for polled operation
  uart_puts("Hello World!\n"); // Write the string to the UART
  while (1)
    ; // Loop forever to prevent program from ending
}
