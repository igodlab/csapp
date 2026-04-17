/*
2.70 ♦♦ 
Write code for the function with the following prototype: 

```c
// Return 1 when x can be represented as an n-bit, 2's-complement number ; 0 otherwise 
// Assume 1 <= n <= w 
int fits_bits(int x, int n);
```
 
Your function should follow the bit-level integer coding rules (page 128). 
*/

#include <assert.h>

const int w = sizeof(int) << 3;

int fits_bits(int x, int n) {
  assert(n > 0 && "Error: entered bit n must be positive");
  assert(w >= n && "Error: entered bit n cannot be greater than the implementation-defined int size 32-bits");

  if (n == w) return 1;   // every int fits in w bits by definition

  // Get two's-comp ranges for n-bits
  int n_tmax = (1 << (n - 1)) - 1;
  int n_tmin = -(1 << (n - 1)); 

  int above_tmin = !((x - n_tmin) & (1 << (w - 1)));  
  int below_tmax = !((n_tmax - x) & (1 << (w - 1)));  

  return above_tmin & below_tmax;
}
