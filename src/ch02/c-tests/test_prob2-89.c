/*
 * test_prob2-89.c — Unity test suite for CSAPP 3e Problem 2.89
 *
 * Build (from src/ch02/tests/):
 *   make test_prob2-89
 * or just:
 *   make
 */

#include "../../../vendor/unity/unity.h"
#include "../c-solutions/prob2-89.h"

#include <limits.h>
#include <math.h>

void setUp(void)    {}   /* required by Unity; nothing to reset here */
void tearDown(void) {}

/* ═══════════════════════════════════════════════════════════════════════════
 * A. (float)x == (float)dx                                    ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 * int→double is always exact (53-bit mantissa covers all 32-bit ints).
 * Rounding to float is therefore identical whether we start from int or double.
 */
void test_A_typical_values(void) {
    TEST_ASSERT_TRUE(condA(0,          (double)0));
    TEST_ASSERT_TRUE(condA(1,          (double)1));
    TEST_ASSERT_TRUE(condA(-1,         (double)-1));
    TEST_ASSERT_TRUE(condA(12345678,   (double)12345678));
    TEST_ASSERT_TRUE(condA(-987654321, (double)-987654321));
}

void test_A_boundary_values(void) {
    TEST_ASSERT_TRUE(condA(INT_MAX, (double)INT_MAX));
    TEST_ASSERT_TRUE(condA(INT_MIN, (double)INT_MIN));
}

/* ═══════════════════════════════════════════════════════════════════════════
 * B. dx - dy == (double)(x-y)                             NOT ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 * Fails when y = INT_MIN: (x - INT_MIN) overflows signed int (UB; wraps on
 * x86 GCC) producing the wrong integer which becomes the wrong double.
 */
void test_B_no_overflow(void) {
    TEST_ASSERT_TRUE(condB(0,         0,        0.0,              0.0));
    TEST_ASSERT_TRUE(condB(INT_MAX,   0,        (double)INT_MAX,  0.0));
    TEST_ASSERT_TRUE(condB(0,         INT_MAX,  0.0,              (double)INT_MAX));
    TEST_ASSERT_TRUE(condB(INT_MIN,   0,        (double)INT_MIN,  0.0));
    TEST_ASSERT_TRUE(condB(INT_MAX,   INT_MAX,  (double)INT_MAX,  (double)INT_MAX));
    TEST_ASSERT_TRUE(condB(100,       -100,     100.0,            -100.0));
}

void test_B_overflow_y_INT_MIN(void) {
    /* x=0, y=INT_MIN: textbook counter-example */
    TEST_ASSERT_FALSE(condB(0,       INT_MIN, 0.0,             (double)INT_MIN));
    TEST_ASSERT_FALSE(condB(1,       INT_MIN, 1.0,             (double)INT_MIN));
    TEST_ASSERT_FALSE(condB(INT_MAX, INT_MIN, (double)INT_MAX, (double)INT_MIN));
}

/* ═══════════════════════════════════════════════════════════════════════════
 * C. (dx+dy)+dz == dx+(dy+dz)                               ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 * Every int32 fits in 53 bits; sums of two int32 fit in 34 bits << 2^53.
 * No rounding ever occurs, so both groupings are bit-identical.
 */
void test_C_associativity_holds(void) {
    double dmax = (double)INT_MAX, dmin = (double)INT_MIN;

    TEST_ASSERT_TRUE(condC(0.0,  0.0,  0.0));
    TEST_ASSERT_TRUE(condC(dmax, dmax, dmax));
    TEST_ASSERT_TRUE(condC(dmin, dmin, dmin));
    TEST_ASSERT_TRUE(condC(dmax, dmin, 0.0));
    TEST_ASSERT_TRUE(condC(dmax, -1.0, dmin));
    TEST_ASSERT_TRUE(condC(1e8,  2e8,  3e8));
    TEST_ASSERT_TRUE(condC(-1e8, -2e8, -3e8));
}

/* ═══════════════════════════════════════════════════════════════════════════
 * D. (dx*dy)*dz == dx*(dy*dz)                           NOT ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 * Products of two int32 can be ~62 bits wide; double stores only 53.
 * The two groupings round at different points and can diverge by 1 ULP.
 *
 * REQUIRES: -mfpmath=sse -msse2 -fno-associative-math -mno-fma
 * (already set in the Makefile — without these the x87 80-bit registers
 *  absorb the rounding difference and the FAIL cases will spuriously PASS.)
 */
void test_D_small_values_exact(void) {
    TEST_ASSERT_TRUE(condD(0.0,   0.0,   0.0));
    TEST_ASSERT_TRUE(condD(1.0,   1.0,   1.0));
    TEST_ASSERT_TRUE(condD(-1.0,  1.0,  -1.0));
    TEST_ASSERT_TRUE(condD(1e3,   1e3,   1e3));   /* product 1e9 < 2^30 << 2^53 */
    TEST_ASSERT_TRUE(condD(1e5,   1e5,   2.0));   /* 2e12 < 2^41 << 2^53       */
}

void test_D_large_products_diverge(void) {
    /* Brute-force verified counter-examples — diverge by 1 ULP on x86-64 SSE */
    TEST_ASSERT_FALSE(condD(-348573226.0, 300159921.0,  -998530546.0));
    TEST_ASSERT_FALSE(condD( 2055324957.0,1819187404.0,  293595697.0));
    TEST_ASSERT_FALSE(condD( -291539692.0,-1606651893.0,-1898132382.0));
}

/* ═══════════════════════════════════════════════════════════════════════════
 * E. dx/dx == dz/dz                                      NOT ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 * IEEE 754: 0.0/0.0 = NaN.  NaN != NaN (not even itself).
 */
void test_E_nonzero_equals_one(void) {
    double dmax = (double)INT_MAX, dmin = (double)INT_MIN;

    TEST_ASSERT_TRUE(condE(1.0,  1.0));
    TEST_ASSERT_TRUE(condE(1.0,  2.0));
    TEST_ASSERT_TRUE(condE(dmax, dmin));
    TEST_ASSERT_TRUE(condE(-1.0, 1.0));
    TEST_ASSERT_TRUE(condE(100.0, -999.0));
}

void test_E_zero_produces_nan(void) {
    /* textbook counter-example: x=0 → dx/dx = NaN ≠ 1.0 = dz/dz */
    TEST_ASSERT_FALSE(condE(0.0, 1.0));
    /* both zero: NaN != NaN */
    TEST_ASSERT_FALSE(condE(0.0, 0.0));
    /* z=0, x nonzero */
    TEST_ASSERT_FALSE(condE((double)INT_MAX, 0.0));
    TEST_ASSERT_FALSE(condE(0.0, (double)INT_MIN));
}

/* ── Runner ───────────────────────────────────────────────────────────────── */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_A_typical_values);
    RUN_TEST(test_A_boundary_values);

    RUN_TEST(test_B_no_overflow);
    RUN_TEST(test_B_overflow_y_INT_MIN);

    RUN_TEST(test_C_associativity_holds);

    RUN_TEST(test_D_small_values_exact);
    RUN_TEST(test_D_large_products_diverge);

    RUN_TEST(test_E_nonzero_equals_one);
    RUN_TEST(test_E_zero_produces_nan);

    UNITY_END();
}
