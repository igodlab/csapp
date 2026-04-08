/*
2.63 ♦♦♦ 
Fill in code for the following C functions. Function srl performs a logical right 
shift using an arithmetic right shift (given by value `xsra`), followed by other operations 
not including right shifts or division. Function sra performs an arithmetic right shift 
using a logical right shift (given by value `xsrl`), followed by other operations not 
including right shifts or division. You may use the computation 8*sizeof (int) to 
determine w, the number of bits in data type int. The shift amount k can range 
from O to w - 1.

```c
unsigned srl (unsigned x , int k) { 
  // Perform shift arithmetically 
  unsigned xsra = (int) x >> k;
  ⋮
}

int sra(int x , int k)  { 
  // Perform shift logically
  int xsrl = (unsigned) x >> k;
  ⋮
} 
```
*/

#include <assert.h>

// Word size
const unsigned w = sizeof(int) << 3;

// logical `x >> k` replica 
unsigned srl(unsigned x, int k) {
  assert(k < w && "Error in custom logical shift: k must be smaller than word size");
  /* Perform shift arithmetically */
  unsigned xsra = (int) x >> k;
  
  /* >>> Complete rest of function >>> */
  // PERF: breaking down mask `((1 << k) - 1) << (w - k)` by parts:
  // `(1 << k)`: shifts unique bit-1 by k-bits to the left eg. `(1 << 3) = 0000 1000`
  // `(1 << k) - 1`: leverages addition (-0b1=~(0b1)+0b1) with carry out of the MSB, leaving k rightmost bits equal to ones eg. `=(+carry)0000 0111`
  // `((1 << k) - 1) << (w - k)`: shifts all k-leftmost-ones to the left by (w-k)-bits eg. `=1110 0000`
  return xsra & ~(((1 << k) - 1) << (w - k));
  /* <<< Complete rest of function <<< */

}

// artithmetic `x >> k` replica 
int sra(int x, int k) {
  assert(k < w && "Error in custom arithmetic shift: k must be smaller than word size");
  /* Perform shift logically */
  int xsrl = (unsigned) x >> k;

  /* >>> Complete rest of function >>> */
  // PERF: step-by-step arithmetic fill
  // 1. Isolate MSB from original x argument then apply double logical !! to get 0x01 if x<0 and 0x00 if x>0
  // 2. Replicate sign bit to w-bits by performing `-msb` which is 0xFFFFFFFF if msb=0x01 and 0x00000000 if msb=0x00
  // 3. Mask k-shifted leftmost bits using the same mask as function `srl`
  int msb = !!(x & (1 << (w - 1)));      // grab most significant bit
  int replicate_msb = -msb; // if msb is positive then
  int mask = ((1 << k) - 1) << (w - k);

  return xsrl | (replicate_msb & mask);
  /* <<< Complete rest of function <<< */
}
