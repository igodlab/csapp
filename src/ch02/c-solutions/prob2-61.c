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

// A. Any bit of x equals 1 — true iff x is not all zeros
int condA(int x) {
  return !!x;
}

// B. Any bit of x equals 0 — true iff x is not all ones
int condB(int x) {
  return !!~x;
}

// C. Any bit in the least significant byte equals 1
int condC(int x) {
  return !!(x & 0xFFu);
}

// D. Any bit in the most significant byte equals 0
int condD(int x) {
  // Isolate MSB by shifting 0xFF (not 0xFFu! Otherwise will implicetly
  // cast x to unsigned as well!) to the most significant byte position,
  // then check if any of those bits are 0 via ~
  return !!~(x | ~(0xFFu << ((sizeof(int) - 1) << 3)));
}
