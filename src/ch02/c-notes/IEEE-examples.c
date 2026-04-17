/*
 * IEEE-754 Examples on how Floating Point Arithmetic is NOT REAL
 *
 * NOTE: Real number arithemtics $\mathbb{R}$ is math so it just KNOWS the answer
 * no need to compute it. But in CS we create a standard to represent floats (closest to real nums)
 *
 */

// Function to convert decimal float to binary float
#include <math.h> // NOTE: must compile with `gcc -lm` flag!
#include <stdio.h>

float d2f(float x_10, int n_bits) {
  float decimal_weight = 1 / pow(2, n_bits);
  float delta = (float)(x_10 - decimal_weight);
  return delta;
}

int main(void) {
  int n_bits = 7;
  printf("1/2^N = %f, where N=%i\n", d2f(0.01f, n_bits), n_bits);

  return 0;
}
