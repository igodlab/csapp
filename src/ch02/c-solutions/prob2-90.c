/*
2.90 ♦ 
You have been assigned the task of writing a C function to compute a floating point 
representation of 2^x . You decide that the best way to do this is to directly construct 
the IEEE single-precision representation of the result. When x is too small, your routine 
will return 0.0. When x is too large, it will return $+\inf$. Fill in the blank portions 
of the code that follows to compute the correct result. Assume the function u2f returns 
a floating-point value having an identical bit representation as its unsigned argument. 

```c
float fpwr2(int x) {
  // Result exponent and fraction
  unsigned exp, frac;
  unsigned u;

  if (x < _____) {
    // Too small. Return 0.0
    exp = _____;
    frac = _____;
  } else if (x < _____) {
    // Denormalized result
    exp = _____;
    frac = _____;
  } else if (x < _____) {
    // Normalized result
    exp = _____;
    frac = _____;
  } else {
    // Too big. Return +inf
    exp = _____;
    frac = _____;
  }

  // Pack exp and frac into 32-bits
  u = exp << 23 | frac;
  // Return as float
  return u2f(u);
}
```
*/

#include <string.h>

float u2f(unsigned u) {
    float f;
    memcpy(&f, &u, sizeof(f));
    return f;
}

int w = sizeof(int) << 3;

float fpwr2(int x) {
  // Result exponent and fraction
  unsigned exp, frac;
  unsigned u;

  // Cases for exponent. The hierarchy goes as
  // Zero < smallest-denorm < largest-denorm < smallest-norm < one < largest-norm
  //
  //                              // single precision k = 8
  //                              // ----------------------
  // E = exponent_k - bias_k      // \in [-126, 127]
  // where:                       
  //   1 <= exponent_k <= 2^k - 2 // \in [1, 254]
  //   bias_k = 2^(k-1) - 1       // = 127
  //
  // Anything smaller than 1 is rounded to zero, thus in IEEE:
  if (x < -149) { // anything smaller than largest-denorm ~ e x 2^-126 = 2^-23 x 2^-126
    // Too small. Return 0.0                              = 2^-149
    exp = 0;
    frac = 0;
  } else if (x < -126) { // anything smaller than largest-denorm ~ (1-e)x2^-126 ~ 2^-126
    // Denormalized result
    exp = 0;
    frac = 1 << (x + 149);
  } else if (x < 128) { // anything smaller than largest-norm ~ (2-e)x2^127 ~ 2^128
    // Normalized result
    exp = x + 127;
    frac = 0;
  } else { 
    // Too big. Return +inf
    exp = 255;
    frac = 0;
  }

  // Pack exp and frac into 32-bits
  u = exp << 23 | frac;
  // Return as float
  return u2f(u);
}
