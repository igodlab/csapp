/*
2.89 ♦ 
We are running programs on a machine where values of type int have a 32-
bit two's-complement representation. Values of type f l oa t  use the 32-bit IEEE 
format, and values of type d oubl e use the 64-bit IEEE format. 
We generate arbitrary integer values x. y. and z, and convert them to values 
of type doubl e as follows: 

```c
// Create some arbitrary values
int x = random();
int y = random();
int z = random();

// Convert to double
double dx = (double) x;
double dy = (double) y;
double dz = (double) z;
```

For each of the following C expressions. you are to indicate whether or not the 
expression always yields 1. If it always yields 1, describe the underlying 
mathematical principles. Otherwise. give an example of arguments that make 
it yield 0. Note that you cannot use an IA32 machine running GCC to test your 
answers, since it would use the 80-bit extended-preci ion representation for both 
float  and double.

A. (float) x == (float) dx 
B. dx - dy == ( double) (x-y) 
C. (dx + dy) + dz == dx + (dy + dz) 
D. (dx * dy) * dz == dx * (dy * dz) 
E. dx / dx == dz / dz
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// NOTE: The five conditions (volatile prevents the compiler from collapsing
// both sides into the same subexpression before comparing them) 
int condA(int x, double dx) {
  return (float)x == (float)dx;
}

int condB(int x, int y, double dx, double dy) {
  /*
   * RHS casts (x-y) to double AFTER integer subtraction.
   * When y = INT_MIN, x-y overflows (UB, but wraps on x86 GCC), giving a
   * wrong integer that becomes a wrong double → differs from dx-dy.
   */
  volatile double lhs = dx - dy;
  volatile double rhs = (double)(x - y);
  return lhs == rhs;
}

int condC(double dx, double dy, double dz) {
  /*
   * Exact because every int32 is exactly representable as double (fits in 53
   * bits), and the sum of any two int32 values fits in 33 bits, still $\ll 2^53$.
   * So no rounding ever occurs -> associativity holds trivially.
   */
  volatile double lhs = (dx + dy) + dz;
  volatile double rhs = dx + (dy + dz);
  return lhs == rhs;
}

// NOTE: This problem exceptionally uses a special gcc set of flags to force
// the compiler not to use optimizations that solve and hide multiplication 
// non-associativity due to compounding errors (condition D) 
int condD(double dx, double dy, double dz) {
  /*
   * Products of two int32 values can be $2^{31}2^{31}\sim 62$ bits wide. A double only holds
   * 53 bits of significand (implicit 1.M and 52 M/mantissa bits), so the product must be rounded.  
   * The two orderings round at different points -> different final results.
   *
   * This REQUIRES strict IEEE 64-bit mode (see compile flags above).
   * With x87 80-bit registers GCC keeps extra precision and the difference
   * vanishes — that is why it "won't be detected with Linux/GCC".
   */
  volatile double lhs = (dx * dy) * dz;
  volatile double rhs = dx * (dy * dz);
  return lhs == rhs;
}

int condE(double dx, double dz) {
  /*
   * IEEE 754: 0/0 = NaN, and NaN != NaN by definition.
   * So if either dx or dz is 0, the corresponding quotient is NaN and the
   * equality is false.
   */
  volatile double lhs = dx / dx;
  volatile double rhs = dz / dz;
  return lhs == rhs;
}

// Helper: hex dump of a double's bit pattern 
void print_bits(const char *name, double v) {
  uint64_t bits;
  memcpy(&bits, &v, 8);
  printf("         %s = %.17g  (0x%016llx)\n",
      name, v, (unsigned long long)bits);
}

