/*
  2.59 ◆◆
  Write a C expression that will yield a word consisting of the least significant byte of 
  x and the remaining bytes of y. For operands x = 0x89ABCDEF and y = 0x76543210, 
  this would give 0x765432EF. 
*/

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int main(void) {
  uint32_t x = 0x89ABCDEF;
  uint32_t y = 0x76543210;

  uint32_t word = (x & 0xFF) | (y & ~0xFFu);
  assert(word == 0x765432EF);
  printf("hex: 0x%08x\n", word);

  return 0;
}
