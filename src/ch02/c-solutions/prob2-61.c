/*
2.61 ♦♦ 
Write C expressions that evaluate to 1 when the following conditions are true and 
to O when they are false. Assume x is of type int. 

A. Any bit of x equals 1 .  
B. Any bit of x equals 0. 
C. Any bit in the least significant byte of x equals 1. 
D. Any bit in the most significant byte of x equals 0. 

Your code should follow the bit-level integer coding rules (page 128), with the 
additional restriction that you may not use equality (==) or inequality (!=) tests.
*/

#include <stdio.h>

int main() {
  int x;
  printf("enter an integer:\n");
  scanf("%i", &x);

  // Conditions
  int condA = !!x;
  int condB = !!~x;

  int xmaskedC = x & 0xFFu;
  int condC = !!xmaskedC;

  int xmaskedD = x | ~((int) (0xFFu << ((sizeof(int) - 1) << 3)));
  int condD = !!~xmaskedD;
  
  printf("A. Any bit of x=%08x equals 1? : %i\n", x, condA);  
  printf("B. Any bit of x=%08x equals 0? : %i\n", x, condB); 
  printf("C. Any bit in the least significant byte of x=%08x equals 1? : %i\n", xmaskedC, condC); 
  printf("D. Any bit in the most significant byte of x=%08x equals 0? : %i\n", xmaskedD, condD); 

  return 0;
}
