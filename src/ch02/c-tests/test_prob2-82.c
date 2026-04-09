/*
 * test_prob2-82.c — Unity test suite for CSAPP 3e Problem 2.82
 *
 * Each expression returns 1 (always true) or 0 (counterexample exists).
 *
 * Summary:
 *   A. NOT always — fails when x=INT_MIN or y=INT_MIN (negation overflows)
 *   B. ALWAYS      — algebraic identity: (x+y)<<4 + y-x == 17y + 15x
 *   C. ALWAYS      — two's-complement identity: ~x+~y+1 == ~(x+y)
 *   D. ALWAYS      — unsigned cast distributes over subtraction
 *   E. ALWAYS      — right-shifting then left-shifting only clears low bits
 */

#include "../../../vendor/unity/unity.h"
#include <limits.h>

char expressionA(int x, int y);
char expressionB(int x, int y);
char expressionC(int x, int y);
char expressionD(int x, int y, int ux, int uy);
char expressionE(int x, int y);

void setUp(void)    {}
void tearDown(void) {}

/* ── A. (x<y) == (-x>-y)  —  NOT always true ───────────────────────────────
 * Fails when x=INT_MIN: -INT_MIN overflows, wrapping back to INT_MIN.
 * So -x == x == INT_MIN, which breaks the expected inequality.
 */
void test_A_typical_holds(void) {
    TEST_ASSERT_TRUE(expressionA(0,    1));
    TEST_ASSERT_TRUE(expressionA(1,    2));
    TEST_ASSERT_TRUE(expressionA(-5,   5));
    TEST_ASSERT_TRUE(expressionA(0,    0));
    TEST_ASSERT_TRUE(expressionA(100, -100));
}

void test_A_fails_when_x_is_INT_MIN(void) {
    TEST_ASSERT_FALSE(expressionA(INT_MIN, 0));
    TEST_ASSERT_FALSE(expressionA(INT_MIN, 1));
    TEST_ASSERT_FALSE(expressionA(INT_MIN, INT_MAX));
}

void test_A_fails_when_y_is_INT_MIN(void) {
    TEST_ASSERT_FALSE(expressionA(0,       INT_MIN));
    TEST_ASSERT_FALSE(expressionA(1,       INT_MIN));
    TEST_ASSERT_FALSE(expressionA(INT_MAX, INT_MIN));
}

/* ── B. ((x+y)<<4) + y-x == 17*y+15*x  —  ALWAYS true ─────────────────────
 * Pure algebraic identity over modular 32-bit arithmetic; holds for all x, y.
 */
void test_B_always_true(void) {
    TEST_ASSERT_TRUE(expressionB(0,       0));
    TEST_ASSERT_TRUE(expressionB(1,       1));
    TEST_ASSERT_TRUE(expressionB(-1,     -1));
    TEST_ASSERT_TRUE(expressionB(INT_MAX, INT_MAX));
    TEST_ASSERT_TRUE(expressionB(INT_MIN, INT_MIN));
    TEST_ASSERT_TRUE(expressionB(INT_MAX, INT_MIN));
    TEST_ASSERT_TRUE(expressionB(INT_MIN, INT_MAX));
    TEST_ASSERT_TRUE(expressionB(100,    -200));
}

/* ── C. ~x+~y+1 == ~(x+y)  —  ALWAYS true ──────────────────────────────────
 * ~z = -z-1 in two's complement, so ~x+~y+1 = (-x-1)+(-y-1)+1 = -(x+y)-1 = ~(x+y).
 */
void test_C_always_true(void) {
    TEST_ASSERT_TRUE(expressionC(0,       0));
    TEST_ASSERT_TRUE(expressionC(1,       1));
    TEST_ASSERT_TRUE(expressionC(-1,     -1));
    TEST_ASSERT_TRUE(expressionC(INT_MAX, INT_MAX));
    TEST_ASSERT_TRUE(expressionC(INT_MIN, INT_MIN));
    TEST_ASSERT_TRUE(expressionC(INT_MAX, INT_MIN));
    TEST_ASSERT_TRUE(expressionC(INT_MIN, INT_MAX));
    TEST_ASSERT_TRUE(expressionC(100,    -200));
}

/* ── D. (ux-uy) == -(unsigned)(y-x)  —  ALWAYS true ────────────────────────
 * Unsigned cast distributes over subtraction in modular arithmetic:
 * -(unsigned)(y-x) = (unsigned)(-(y-x)) = (unsigned)(x-y) = ux-uy.
 */
void test_D_always_true(void) {
    unsigned ux, uy;

    ux = (unsigned)0;       uy = (unsigned)0;
    TEST_ASSERT_TRUE(expressionD(0, 0, ux, uy));

    ux = (unsigned)1;       uy = (unsigned)0;
    TEST_ASSERT_TRUE(expressionD(1, 0, ux, uy));

    ux = (unsigned)INT_MAX; uy = (unsigned)INT_MIN;
    TEST_ASSERT_TRUE(expressionD(INT_MAX, INT_MIN, ux, uy));

    ux = (unsigned)INT_MIN; uy = (unsigned)INT_MAX;
    TEST_ASSERT_TRUE(expressionD(INT_MIN, INT_MAX, ux, uy));

    ux = (unsigned)-1;      uy = (unsigned)1;
    TEST_ASSERT_TRUE(expressionD(-1, 1, ux, uy));
}

/* ── E. (x>>2)<<2 <= x  —  ALWAYS true ─────────────────────────────────────
 * Right-shifting clears the 2 low bits; left-shifting puts zeros back.
 * The result equals x with its two LSBs zeroed, which is always <= x
 * for both positive and negative values in two's-complement arithmetic.
 */
void test_E_always_true(void) {
    TEST_ASSERT_TRUE(expressionE(0,       0));
    TEST_ASSERT_TRUE(expressionE(1,       0));
    TEST_ASSERT_TRUE(expressionE(-1,      0));
    TEST_ASSERT_TRUE(expressionE(INT_MAX, 0));
    TEST_ASSERT_TRUE(expressionE(INT_MIN, 0));
    TEST_ASSERT_TRUE(expressionE(7,       0));   /* 0b111  → 0b100 = 4  <= 7  */
    TEST_ASSERT_TRUE(expressionE(-5,      0));   /* clears LSBs, stays <= -5  */
    TEST_ASSERT_TRUE(expressionE(100,     0));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_A_typical_holds);
    RUN_TEST(test_A_fails_when_x_is_INT_MIN);
    RUN_TEST(test_A_fails_when_y_is_INT_MIN);

    RUN_TEST(test_B_always_true);
    RUN_TEST(test_C_always_true);
    RUN_TEST(test_D_always_true);
    RUN_TEST(test_E_always_true);

    UNITY_END();
}
