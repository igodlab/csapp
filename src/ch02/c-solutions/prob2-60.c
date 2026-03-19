/*  
2.60 ♦♦ 
Suppose we number the bytes in a w-bit word from O (least significant) to w/8 - 1 
(most significant). Write code for the following C function, which will return an 
unsigned value in which byte i of argument x has been replaced by byte b: 

`unsigned replace_byte (unsigned x, int i ,  unsigned char b);`

Here are some examples showing how the function should work: 

```
replace_byte(0x12345678 , 2 ,  0xAB) --> 0x12AB5678 
replace_byte(0x12345678 , 0 ,  0xAB) --> 0x123456AB 
```
*/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

unsigned replace_byte(unsigned x, int i, unsigned char b) {
  // Make sure i isn't out of range. Since we start counting on 0 to (w/8 - 1).
  assert(i >= 0);
  assert(i <= (sizeof(unsigned) - 1));

  // Recall that `x << k` yields `x * 2^k`, thus `x << 3`, moves in byte-steps 
  unsigned mask = 0xFFu << (i << 3);
  unsigned replacement_byte = ((unsigned) b) << (i << 3);
  return (x & ~mask) | replacement_byte;
}

int main(void) {
  assert(replace_byte(0x12345678, 2, 0xAB) == 0x12AB5678u);
  assert(replace_byte(0x12345678, 0, 0xAB) == 0x123456ABu);
  printf("0x%08x\n", replace_byte(0x12345678u, 2, 0xAB));
  printf("0x%08x\n", replace_byte(0x12345678u, 0, 0xAB));
  return 0; 
}


