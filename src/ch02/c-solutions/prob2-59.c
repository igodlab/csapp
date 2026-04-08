/*
  2.59 ◆◆
  Write a C expression that will yield a word consisting of the least significant byte of 
  x and the remaining bytes of y. For operands x = 0x89ABCDEF and y = 0x76543210, 
  this would give 0x765432EF. 
*/

unsigned int merge_lsb(unsigned int x, unsigned int y) {
  return (x & 0xFF) | (y & ~0xFFu);
}
