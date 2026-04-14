#include <stdio.h>
#include "utils.h"

void print_bits(unsigned int n) {
  int w = sizeof(n) << 3;

  for (int i = w - 1; i >= 0; i--) {
    // Shift bit at position 'i' to the 0th position and mask it
    int bit = (n >> i) & 1;
    printf("%d", bit);

    // Optional: add a space every 8 bits for readability
    if (i % 8 == 0 && i != 0) printf(" ");
  }
  printf("\n");
}
