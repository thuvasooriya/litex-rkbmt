#include "uart.h"

void main() {
  // UART0_FCR = UARTFCR_FFENA;   // Set the FIFO for polled operation
  uart_puts("Hello World!\n"); // Write the string to the UART

  // Perform floating-point addition and multiplication
  float a = 1.1111f; // Single-precision float with 'f' suffix
  float b = 2.4356f;
  float c = a + b; // Addition: 1.0 + 2.0 = 3.0
  float d = a * b; // Multiplication: 1.0 * 2.0 = 2.0

  // Store results in volatile variables for GDB inspection
  volatile float result_add = c;
  volatile float result_mul = d;

  uart_puts("Float Calculations Done!\n"); // Write the string to the UART

  double e = 1.1111; // Double-precision float
  double f = 2.4356;
  double g = e + f; // Addition: 1.0 + 2.0 = 3.0
  double h = e * f; // Multiplication: 1.0 * 2.0 = 2.0

  // Store results in volatile variables for GDB inspection
  volatile double result_add_d = g;
  volatile double result_mul_d = h;

  uart_puts("Double Calculations Done!\n"); // Write the string to the UART
  while (1)
    ; // Loop forever to prevent program from ending
}
