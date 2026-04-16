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
const int mantissa_bits = 23;

typedef union {
  float f;
  uint32_t u;
} float_bits;


// We'll express our number in the form: 
// $$
// b = \sum_{i=-n}^E b_i\times 2^i
// $$
int weighted_digits(uint32_t raw_mantissa, int E, int sign) {
  // Account for implicit one in significand: 1.mantissa
  int whole_part = 1 << E; // Set its value with the largest exponent ie. 1 x 2^E
  int weight_exp = E - 1; // This is the second largest exponent base (only) ie. 2^(E-1)
  float frac_part = 0.0f;

  printf("\nimplicit digit = 1x2^%i", E);
  for (int i = mantissa_bits - 1; i >= 0; i--) {
    int positional_bit = (raw_mantissa >> i) & 1; // positional bit b_i
    printf("\ncounter = %i, %01ix2^%i", i, positional_bit, weight_exp);

    if (weight_exp >= 0)
    {
      printf(", 2^i = %i", positional_bit << weight_exp);
      whole_part += positional_bit << weight_exp;
    } else
    {
      printf(", 2^-i = %f", (float)positional_bit / (1 << (~weight_exp + 1)));
      frac_part += (float)positional_bit / (1 << (~weight_exp + 1));
    }
  }

  weight_exp--;
  if (1 == sign) 
  {
    printf("\nsign is negative");
    printf("\nwhole_part = %i", -whole_part);
    printf("\nfrac_part = %f\n", frac_part);
    return -whole_part;
  } else
  {
    printf("\nsign is positive");
    printf("\nwhole_part = %i", whole_part);
    printf("\nfrac_part = %f\n", frac_part);
    return whole_part;
  }
}

int float_f2i(float_bits f) {
  int S = f.u >> (w - 1);
  unsigned exponent_u = f.u >> (w - 1 - exponent_bits);
  int exponent = exponent_u & 0xFFu;
  int E = exponent - bias;
  int mantissa = f.u & ((1u << mantissa_bits) - 1);
  
  int reconstructed_bits = (S << (w - 1)) | ((E + bias) << (w - 1 - exponent_bits)) | ((unsigned)mantissa);
  // return reconstructed_bits;

  // Check if exponent is negative (rounds to zero) ie. E = exponent_k - bias_k < 0
  if (E < 0) 
    return 0;

  // Check overflow, large exponent ie. E = exponent_k - bias_k > 31
  // why 31? Because 2^31 = 2147483648 (48 off due to error) 
  // which overflows INT_MAX=2147483647 
  if (E > 31)
    return INT_MAX;
  
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
  // Exponent within normalized range gets rounded to floor
  else {
    return weighted_digits(f.u, E, S);
    // return cast_conversion(f.u, E);
  }
}

int main(void) {
  float_bits f;
  scanf("%f", &f.f);
  print_bits(f.u);
  // print_bits(float_f2i(f));
  printf("%i\n", float_f2i(f));
  return 0;
}
