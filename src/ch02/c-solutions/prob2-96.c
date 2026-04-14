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

int weighted_digits() {
  return 0;
}

int float_f2i(float_bits f) {
  unsigned exponent_mask = 0xFFu << (w - 1 - exponent_bits);
  int S = f.u >> (w - 1);
  int exponent = (f.u & exponent_mask) >> (w - 1 - exponent_bits);
  int E = exponent - bias;
  int mantissa = f.u & ((1u << fraction_bits) - 1);
  
  int reconstructed_bits = (S << (w - 1)) | ((E + bias) << (w - 1 - exponent_bits)) | ((unsigned)mantissa);
  // return reconstructed_bits;

  // 3402823466385288598117042
  // Denormalized numbers -> all round to zero
  if (0 == exponent) 
  {
    return 0;
  } 
  // Positive (negative) infinity returns TMax (TMin)
  else if (1 == !!~(exponent | ~((1 << exponent_bits) - 1))) 
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
  //
  // 340282346638528859811704200000000000000 (value actually stored in float)
  // +       3361471140188295800000000000000 (error due to conversion)
  // ---------------------------------------
  // 340282350000000000000000000000000000000 (decimal representation of 0X7F7FFFFF)
  //
  else {
    return reconstructed_bits;
  }
}

void print_bits(unsigned int n) {
    for (int i = w - 1; i >= 0; i--) {
        // Shift bit at position 'i' to the 0th position and mask it
        int bit = (n >> i) & 1;
        printf("%d", bit);
        
        // Optional: add a space every 8 bits for readability
        if (i % 8 == 0 && i != 0) printf(" ");
    }
    printf("\n");
}

int main(void) {
  float_bits f;
  scanf("%f", &f.f);
  printf("FLT_MIN=%038f\n", FLT_MIN);
  printf("FLT_MAX=%038f\n", FLT_MAX);
  printf("FLT_MAX=%038f\n", FLT_MAX - 2e31f);
  print_bits(f.u);
  print_bits(float_f2i(f));
  printf("%i", float_f2i(f));
  return 0;
}
