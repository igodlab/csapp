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
#include <stdio.h>

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

// --- Test framework ---
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

// --- srl tests ---
void test_srl(void) {
    // Basic: positive value, no sign bit confusion
    TEST("srl: 0x00000001 >> 0",  srl(0x00000001, 0) == 0x00000001);
    TEST("srl: 0x00000001 >> 1",  srl(0x00000001, 1) == 0x00000000);

    // High bit set — critical case where arithmetic vs logical differs
    TEST("srl: 0x80000000 >> 1",  srl(0x80000000, 1) == 0x40000000);  // must NOT sign extend
    TEST("srl: 0x80000000 >> 4",  srl(0x80000000, 4) == 0x08000000);
    TEST("srl: 0xFFFFFFFF >> 1",  srl(0xFFFFFFFF, 1) == 0x7FFFFFFF);  // top bit must be 0
    TEST("srl: 0xFFFFFFFF >> 4",  srl(0xFFFFFFFF, 4) == 0x0FFFFFFF);
    TEST("srl: 0xFFFFFFFF >> 8",  srl(0xFFFFFFFF, 8) == 0x00FFFFFF);

    // Shift by 0: identity
    TEST("srl: 0x89ABCDEF >> 0",  srl(0x89ABCDEF, 0) == 0x89ABCDEF);

    // Shift by w-1: only MSB survives, must be 0 or 1
    TEST("srl: 0x80000000 >> 31", srl(0x80000000, 31) == 0x00000001);
    TEST("srl: 0x7FFFFFFF >> 31", srl(0x7FFFFFFF, 31) == 0x00000000);

    // All zeros
    TEST("srl: 0x00000000 >> 4",  srl(0x00000000, 4) == 0x00000000);
}

// --- sra tests ---
void test_sra(void) {
    // Positive value: sra and srl should agree (MSB is 0, no sign extension)
    TEST("sra: 0x00000001 >> 0",  sra(0x00000001, 0) == 0x00000001);
    TEST("sra: 0x7FFFFFFF >> 1",  sra(0x7FFFFFFF, 1) == 0x3FFFFFFF);  // top bit must be 0
    TEST("sra: 0x00000008 >> 3",  sra(0x00000008, 3) == 0x00000001);

    // Negative value: must sign extend with 1s
    TEST("sra: 0x80000000 >> 1",  sra(0x80000000, 1) == 0xC0000000);  // must sign extend
    TEST("sra: 0x80000000 >> 4",  sra(0x80000000, 4) == 0xF8000000);
    TEST("sra: 0xFFFFFFFF >> 1",  sra(0xFFFFFFFF, 1) == 0xFFFFFFFF);  // all ones stays all ones
    TEST("sra: 0xFFFFFFFF >> 4",  sra(0xFFFFFFFF, 4) == 0xFFFFFFFF);
    TEST("sra: 0x80000008 >> 4",  sra(0x80000008, 4) == 0xF8000000);  // your original bug case

    // Shift by 0: identity
    TEST("sra: 0x89ABCDEF >> 0",  sra(0x89ABCDEF, 0) == 0x89ABCDEF);

    // Shift by w-1: result is all 1s for negative, 0 for positive
    TEST("sra: 0x80000000 >> 31", sra(0x80000000, 31) == 0xFFFFFFFF);
    TEST("sra: 0x7FFFFFFF >> 31", sra(0x7FFFFFFF, 31) == 0x00000000);

    // All zeros
    TEST("sra: 0x00000000 >> 4",  sra(0x00000000, 4) == 0x00000000);
}

// --- srl vs sra agreement on positive values ---
void test_srl_sra_agree(void) {
    // For positive values (MSB=0), srl and sra must produce identical results
    TEST("agree: 0x00000001 >> 1", srl(0x00000001, 1) == (unsigned)sra(0x00000001, 1));
    TEST("agree: 0x7FFFFFFF >> 4", srl(0x7FFFFFFF, 4) == (unsigned)sra(0x7FFFFFFF, 4));
    TEST("agree: 0x12345678 >> 8", srl(0x12345678, 8) == (unsigned)sra(0x12345678, 8));
}

int main(void) {
    test_srl();
    test_sra();
    test_srl_sra_agree();

    printf("\n%d/%d tests passed\n", tests_run - tests_failed, tests_run);
    return tests_failed > 0 ? 1 : 0;
}
