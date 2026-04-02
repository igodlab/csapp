/*
2.82 ♦ 
We are running programs where values of type in t are 32 bits. They are repre­
sented in two's complement, and they are right shifted arithmetically. Values of 
type unsigned are also 32 bits. 

We generate arbitrary values x and y, and convert them to unsigned values as 
follows: 

```c
// Create some arbitrary values
int x = random();
int y = random();
// Convert to unsigned
unsigned ux = (unsigend) x;
unsigned uy = (unsigned) y;
```

For each of the following C expressions, you are to indicate whether or 
not the expression always yields 1. If it always yields 1, describe the underlying 
mathematical principles. Otherwise, give an example of arguments that make it yield 0.

- A. `(x<y) == (-x>-y)`
- B. `((x+y)<<4) + y-x == 17*y+15*x`
- C. `~x+~y+1 == ~(x+y)`
- D. `(ux-uy) == -(unsigned)(y-x)`
- E. `((x >> 2) << 2) <= x`
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int expressionA(int x, int y) {
  // No! Due to Two's-complement assymmetry if x or y=-INT_MIN overflows!
  // because -(-2^(w-1))=INT_MAX+1 which exceeds the TMax limit: 2^(w-1)-1
  return (x<y) == (-x>-y);
}

int expressionB(int x, int y) {
  // Yes! Math shows the equivalence
  // (x+y)*2^4 + y-x = y*(2^4+1) + x*(2^4-1) = 17*y + 15*x
  return ((x+y)<<4) + y-x == 17*y+15*x;
}

int expressionC(int x, int y) {
  // Yes! Two's-complement sign inversion -|z| = ~|z| + 1, thus ~|z| = -|z| - 1
  // -|x|-1 - |y|-1 + 1 = -(|x|+|y|)-1 = ~|x+y|
  return ~x+~y+1 == ~(x+y);
}

int expressionD(int x, int y, int ux, int uy) {
  // Yes! Type casting acts as a distributed operator under addition, thus:
  // -(unsigned)(y-x) = -((unsigned)y - (unsigned)x) = ux - uy
  return (ux-uy) == -(unsigned)(y-x);
}

int expressionE(int x, int y) {
  // Yes! Math shows the equvalence:
  // x*2^(-2) * 2^2 = x
  return ((x >> 2) << 2) <= x;
}

int main(void) {
  // Create some arbitrary values
  // int x = rand();
  // int y = rand();
  int x = INT_MIN;
  int y = 0;
  // Convert to unsigned
  unsigned ux = (unsigned) x;
  unsigned uy = (unsigned) y;

  // print
  printf("x=%i, y=%i\n", x, y);
  printf("ux=%i, uy=%i\n", ux, uy);

  printf("Expressions:\n\n");
  printf("A. 0x%02hhx\n", expressionA(x, y));
  printf("B. 0x%02hhx\n", expressionB(x, y));
  printf("C. 0x%02hhx\n", expressionC(x, y));
  printf("D. 0x%02hhx\n", expressionD(x, y, ux, uy));
  printf("E. 0x%02hhx\n", expressionE(x, y));
  return 0;
}
