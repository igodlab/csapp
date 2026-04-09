/*
2.84 ♦ 
Fill in the return value for the following procedure, which tests whether its first 
argument is less than or equal to its second. Assume the function f2u returns an 
unsigned 32-bit number having the same bit representation as its floating-point 
argument. You can assume that neither argument is NaN. The two flavors of zero, 
+0 and -0, are considered equal. 

```c
int float_le(float x, float y) {
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);

  // Get the sign bits
  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;

  // Give an expression using only ux, uy, sx and sy
}
```
*/

#include <string.h>

// Extra f2u custom function (extra part not what the problem asks to solve)
unsigned f2u(float x) {
  unsigned u;
  memcpy(&u, &x, sizeof(u));
  return u;
}

int float_le(float x, float y) {
  unsigned ux = f2u(x);
  unsigned uy = f2u(y);

  // Get the sign bits
  unsigned sx = ux >> 31;
  unsigned sy = uy >> 31;

  // mask signed bit
  unsigned mask_sb = ~(1u << ((sizeof(int) << 3) - 1));

  // Give an expression using only ux, uy, sx and sy
  //
  // Should return whether its first argument is less than or equal to its second argument
  // which can be done becuase the ordering of float is preserved and transalted to an unsigned scale
  // We must check the following cases:
  //  > both x,y=0
  //  > |x| <= |y|, which occurs in any of the following sub cases:
  //    > if x < 0 and y > 0
  //    > if x > 0 and y > 0 and x <= y
  //    > if x < 0 and y < 0 and x >= y
  return 
    ((ux & mask_sb) == 0 && (uy & mask_sb) == 0) ||
    (sx && !sy) ||
    (!sx && !sy && ux <= uy) ||
    (sx && sy && ux >= uy);
}
