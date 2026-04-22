/*
 * test_prob2-90.c — Unity test suite for CSAPP 3e Problem 2.90
 *
 * fpwr2(x) computes 2^x as a float by directly constructing the IEEE bit pattern.
 *
 * IEEE single-precision ranges:
 *   x < -149           → 0.0   (underflow, below smallest denorm)
 *   -149 <= x < -126   → denormalized
 *   -126 <= x < 128    → normalized
 *   x >= 128           → +inf  (overflow)
 */

#include "../../../vendor/unity/unity.h"
#include <math.h>
#include <string.h>

float fpwr2(int x);

static unsigned f2u(float f) {
    unsigned u;
    memcpy(&u, &f, sizeof(u));
    return u;
}

void setUp(void)    {}
void tearDown(void) {}

/* ── Too small: return 0.0 ───────────────────────────────────────────────── */
void test_too_small_returns_zero(void) {
    TEST_ASSERT_EQUAL_INT(0x00000000, f2u(fpwr2(-150)));
    TEST_ASSERT_EQUAL_INT(0x00000000, f2u(fpwr2(-200)));
    TEST_ASSERT_EQUAL_INT(0x00000000, f2u(fpwr2(-10000)));
}

void test_boundary_below_denorm(void) {
    /* x=-150 is just below the smallest denorm */
    TEST_ASSERT_EQUAL_INT(0x00000000, f2u(fpwr2(-150)));
}

/* ── Denormalized range: -149 <= x < -126 ────────────────────────────────── */
void test_denorm_smallest(void) {
    /* x=-149: exp=0, frac=1<<0=0x000001 */
    TEST_ASSERT_EQUAL_INT(0x00000001, f2u(fpwr2(-149)));
}

void test_denorm_middle(void) {
    /* x=-140: frac=1<<(-140+149)=1<<9=0x200 */
    TEST_ASSERT_EQUAL_INT(0x00000200, f2u(fpwr2(-140)));
}

void test_denorm_largest(void) {
    /* x=-127: frac=1<<(-127+149)=1<<22=0x400000 */
    TEST_ASSERT_EQUAL_INT(0x00400000, f2u(fpwr2(-127)));
}

/* ── Normalized range: -126 <= x < 128 ──────────────────────────────────── */
void test_norm_smallest(void) {
    /* x=-126: exp=-126+127=1, frac=0 → 0x00800000 */
    TEST_ASSERT_EQUAL_INT(0x00800000, f2u(fpwr2(-126)));
}

void test_norm_zero_exponent(void) {
    /* x=0: 2^0=1.0f → exp=127, frac=0 → 0x3F800000 */
    TEST_ASSERT_EQUAL_INT(0x3F800000, f2u(fpwr2(0)));
}

void test_norm_x_equals_1(void) {
    /* x=1: 2^1=2.0f → exp=128, frac=0 → 0x40000000 */
    TEST_ASSERT_EQUAL_INT(0x40000000, f2u(fpwr2(1)));
}

void test_norm_x_equals_minus1(void) {
    /* x=-1: 2^-1=0.5f → exp=126, frac=0 → 0x3F000000 */
    TEST_ASSERT_EQUAL_INT(0x3F000000, f2u(fpwr2(-1)));
}

void test_norm_various(void) {
    /* cross-check against powf for a range of exponents */
    int cases[] = {-126, -100, -50, -10, -1, 0, 1, 10, 50, 100, 127};
    for (int i = 0; i < 11; i++) {
        int x = cases[i];
        TEST_ASSERT_EQUAL_INT(f2u(powf(2.0f, (float)x)), f2u(fpwr2(x)));
    }
}

void test_norm_largest(void) {
    /* x=127: exp=254, frac=0 → 0x7F000000 */
    TEST_ASSERT_EQUAL_INT(0x7F000000, f2u(fpwr2(127)));
}

/* ── Too big: return +inf ────────────────────────────────────────────────── */
void test_too_big_returns_inf(void) {
    /* +inf: exp=0xFF, frac=0 → 0x7F800000 */
    TEST_ASSERT_EQUAL_INT(0x7F800000, f2u(fpwr2(128)));
    TEST_ASSERT_EQUAL_INT(0x7F800000, f2u(fpwr2(200)));
    TEST_ASSERT_EQUAL_INT(0x7F800000, f2u(fpwr2(10000)));
}

void test_boundary_above_norm(void) {
    /* x=128 is the first overflow */
    TEST_ASSERT_EQUAL_INT(0x7F800000, f2u(fpwr2(128)));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_too_small_returns_zero);
    RUN_TEST(test_boundary_below_denorm);

    RUN_TEST(test_denorm_smallest);
    RUN_TEST(test_denorm_middle);
    RUN_TEST(test_denorm_largest);

    RUN_TEST(test_norm_smallest);
    RUN_TEST(test_norm_zero_exponent);
    RUN_TEST(test_norm_x_equals_1);
    RUN_TEST(test_norm_x_equals_minus1);
    RUN_TEST(test_norm_various);
    RUN_TEST(test_norm_largest);

    RUN_TEST(test_too_big_returns_inf);
    RUN_TEST(test_boundary_above_norm);

    UNITY_END();
}
