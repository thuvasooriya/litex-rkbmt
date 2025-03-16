#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void calc(void);
int32_t add_integers(int32_t a, int32_t b);
float add_floats(float a, float b);
float multiply_floats(float a, float b);
void print_float_details(float f);

void calc(void) {
  printf("integer addition\n");
  int int_sum = add_integers(5, 10); // Expected: 15
  printf("integer sum: %d\n", int_sum);

  // test floating-point addition
  printf("floating-point addition\n");
  float float_sum = add_floats(3.5f, 2.5f); // Expected: 6.0
  print_float_details(float_sum);
  printf("floating-point multiplication\n");
  float float_product = multiply_floats(3.5f, 2.5f); // Expected: 8.75
  print_float_details(float_product);
}

int32_t add_integers(int32_t a, int32_t b) { return a + b; }

float add_floats(float a, float b) { return a + b; }

float multiply_floats(float a, float b) { return a * b; }

void print_float_details(float f) {
  // convert float to raw IEEE 754 bits
  uint32_t bits;
  memcpy(&bits, &f, sizeof(float)); // safely copy float bits into uint32_t

  // print memory address
  printf("memory address: %p\n", (void *)&f);

  // print IEEE 754 representation in hexadecimal
  printf("bit representation (hex): %08x\n", bits);

  // print bit representation in binary
  printf("bit representation (binary): ");
  for (int i = 31; i >= 0; i--) {
    printf("%d", (bits >> i) & 1);
    if (i % 4 == 0)
      printf(" "); // spacing for readability
  }
  printf("\n");

  // print the float value
  printf("float value: %.6f\n", f);
}
