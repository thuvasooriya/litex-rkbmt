#pragma once

#define UART0_BASE 0x10000000 // UART0 base address for QEMU virt machine
// Use a datasheet for a 16550 UART
// For example: https://www.ti.com/lit/ds/symlink/tl16c550d.pdf
#define REG(base, offset) ((*((volatile unsigned char *)(base + offset))))
#define UART0_DR REG(UART0_BASE, 0x00)
#define UART0_FCR REG(UART0_BASE, 0x02)
#define UART0_LSR REG(UART0_BASE, 0x05)
#define UARTFCR_FFENA 0x01 // UART FIFO Control Register enable bit
#define UARTLSR_THRE                                                           \
  0x20 // UART Line Status Register Transmit Hold Register Empty bit
#define UART0_FF_THR_EMPTY (UART0_LSR & UARTLSR_THRE)

// void uart_putc(char c) {
//   while (!UART0_FF_THR_EMPTY)
//     ;           // Wait until the FIFO holding register is empty
//   UART0_DR = c; // Write character to transmitter register
// }
//
// void uart_puts(const char *str) {
//   while (*str) {       // Loop until value at string pointer is zero
//     uart_putc(*str++); // Write the character and increment pointer
//   }
// }
//

// Function to write a character to UART0
void uart_putc(char c) {
  volatile char *uart = (volatile char *)UART0_BASE;
  *uart = c; // Write character to UART
}

// Function to write a string to UART0
void uart_puts(const char *s) {
  while (*s) {
    uart_putc(*s++); // Write each character until null terminator
  }
}
