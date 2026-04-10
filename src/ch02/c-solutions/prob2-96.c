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

#include <stdint.h>
#include <stdio.h>


const int w = sizeof(float) << 3;

typedef union {
  float f;
  uint32_t u;
} float_bits;

int float_f2i(float_bits f) {
  unsigned mask_sign = 1u << (w - 1);
  int s = !!(f.u & mask_sign);
  // int mantissa = 
  return s;
}

int main(void) {
  float_bits f;
  scanf("%f", &f.f);
  printf("sign of s(f)=%i", float_f2i(f));
  return 0;
}
