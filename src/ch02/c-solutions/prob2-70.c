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
#include <limits.h>
#include <stdio.h>

const unsigned w = sizeof(int) << 3;

int fits_bits(int x, int n) {
  assert(n > 0 && "Error: entered bit n must be positive");
  assert(w >= n && "Error: entered bit n cannot be greater than the implementation-defined int size 32-bits");

  // Get two's-comp ranges for n-bits
  int n_tmax = (1 << (n - 1)) - 1;
  int n_tmin = -(1 << (n - 1)); 

  int above_tmin = !((x - n_tmin) & (1 << (w - 1)));  
  int below_tmax = !((n_tmax - x) & (1 << (w - 1)));  

  return above_tmin & below_tmax;
}

static int tests_run = 0;
static int tests_failed = 0;

#define TEST(desc, expr) do { \
  tests_run++; \
  if (!(expr)) { \
    printf("FAIL [%s] line %d: %s\n", desc, __LINE__, #expr); \
    tests_failed++; \
  } else { \
    printf("PASS [%s]\n", desc); \
  } \
} while(0)

void test_fits_bits(void) {
  // n=1: only representable value is 0 and -1
  TEST("n=1: x=0", fits_bits(0,  1) == 1);
  TEST("n=1: x=-1", fits_bits(-1, 1) == 1);
  TEST("n=1: x=1", fits_bits(1,  1) == 0);

  // n=4: range [-8, 7]
  TEST("n=4: x=7", fits_bits(7,   4) == 1);
  TEST("n=4: x=-8", fits_bits(-8,  4) == 1);
  TEST("n=4: x=8", fits_bits(8,   4) == 0); 
  TEST("n=4: x=-9", fits_bits(-9,  4) == 0);
  TEST("n=4: x=0", fits_bits(0,   4) == 1);

  // n=8: range [-128, 127]
  TEST("n=8: x=127", fits_bits(127,  8) == 1);
  TEST("n=8: x=-128", fits_bits(-128, 8) == 1);
  TEST("n=8: x=128", fits_bits(128,  8) == 0);
  TEST("n=8: x=-129", fits_bits(-129, 8) == 0);

  // n=16: range [-32768, 32767]
  TEST("n=16: x=32767", fits_bits(32767,  16) == 1);
  TEST("n=16: x=-32768", fits_bits(-32768, 16) == 1);
  TEST("n=16: x=128", fits_bits(32768,  16) == 0);
  TEST("n=16: x=-32769", fits_bits(-32769, 16) == 0);

  // n=w: any int fits
  TEST("n=w: INT_MAX", fits_bits(INT_MAX, 32) == 1);
  TEST("n=w: INT_MIN", fits_bits(INT_MIN, 32) == 1);

  // n=32: boundary
  TEST("n=32: x=0", fits_bits(0, 32) == 1);
}

int main(void) {
  test_fits_bits();
  printf("\n%d/%d tests passed\n", tests_run - tests_failed, tests_run);
  return tests_failed > 0 ? 1 : 0;
}
