/*
2.96 ◆◆◆◆
Following the bit-level floating-point coding rules, implement the function with 
the following prototype:

```c
// Compute (int) f. 
// If conversion causes overflow or f is NaN, then return 0x80000000.
int float_f2i(float_bits f); 
```

For floating-point number f, this function computes `(int) f`. If f is NaN, your 
function should round toward zero. If f cannot be represented as an integer (eg. it is out 
of range, or il is NaN), then the function should return `0x80000000`. 
Test your function by evaluating it for all 2^32 values of argument f and com­
paring the result to what would be obtained using your machine's floating-point 
operations.
*/

#include "utils.h"
#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>

const int w = sizeof(float) << 3;
const int exponent_bits = 8;
const int bias = (1 << (exponent_bits - 1)) - 1;
const int fraction_bits = 23;

typedef union {
  float f;
  uint32_t u;
} float_bits;

int weighted_digits(uint32_t raw_x, int E) {
  int whole_part = 1 << E; // Account for implicit one in significand: 1.fraction
  float frac_part = 0.0f;
  int weight_exp = E - 1;
  for (int i = fraction_bits - 1; i >= 0; i--) {
    int positional_bit = (raw_x >> i) & 1;
    // Check if exponent should be positive or negative
    // printf("\ncounter = %i, E=%i", i, weight_exp);
    if (i > fraction_bits - 1 - E) 
    {
      whole_part += positional_bit << weight_exp;
      weight_exp--;
    } else
    {
      frac_part += (float)positional_bit / (1 << ((int)(~weight_exp + 1)));
      weight_exp--;
    }
  }

  printf("\nwhole_part=%i\n", whole_part);
  printf("frac_part=%f\n", frac_part);
  if (frac_part > 0.5) 
  {
    return whole_part + 1;
  } else
  {
    return whole_part;
  }
}

int float_f2i(float_bits f) {
  int S = f.u >> (w - 1);
  unsigned int exponent_u = f.u >> (w - 1 - exponent_bits);
  int exponent = exponent_u & 0xFFu;
  int E = exponent - bias;
  int mantissa = f.u & ((1u << fraction_bits) - 1);
  
  int reconstructed_bits = (S << (w - 1)) | ((E + bias) << (w - 1 - exponent_bits)) | ((unsigned)mantissa);
  // return reconstructed_bits;

  // Denormalized numbers -> all round to zero
  if (0 == exponent) 
  {
    return 0;
  } 
  // All ones exponent 0xFF -> Positive (negative) infinity returns TMax (TMin)
  else if (0xFFu == exponent) 
  {
    if (S == 1) {
      return INT_MAX;
    } else {
      return INT_MIN;
    }
  }
  // Exponent within range gets rounded to nearest unsigned integer
  //
  // 340282346638528859811704183484516925440 (FLT_MAX)
  else {
    return weighted_digits(f.u, E) | (S << (w - 1));
  }
}

int main(void) {
  float_bits f;
  scanf("%f", &f.f);
  print_bits(f.u);
  print_bits(float_f2i(f));
  printf("%i\n", float_f2i(f));
  return 0;
}
