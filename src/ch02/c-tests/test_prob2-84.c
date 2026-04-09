/*
 * test_prob2-84.c — Unity test suite for CSAPP 3e Problem 2.84
 *
 * float_le(x, y) returns 1 when x <= y, 0 otherwise.
 * Neither argument is NaN. +0 and -0 are considered equal.
 */

#include "../../../vendor/unity/unity.h"
#include <float.h>

int float_le(float x, float y);

void setUp(void)    {}
void tearDown(void) {}

/* ── Zero handling: +0 and -0 are equal ─────────────────────────────────── */
void test_zero_equality(void) {
    TEST_ASSERT_TRUE(float_le( 0.0f,  0.0f));
    TEST_ASSERT_TRUE(float_le(-0.0f,  0.0f));
    TEST_ASSERT_TRUE(float_le( 0.0f, -0.0f));
    TEST_ASSERT_TRUE(float_le(-0.0f, -0.0f));
}

/* ── Both positive ───────────────────────────────────────────────────────── */
void test_both_positive(void) {
    TEST_ASSERT_TRUE(float_le(1.0f,   2.0f));
    TEST_ASSERT_TRUE(float_le(1.0f,   1.0f));   /* equal */
    TEST_ASSERT_FALSE(float_le(2.0f,  1.0f));
    TEST_ASSERT_TRUE(float_le(0.5f,   1.5f));
    TEST_ASSERT_FALSE(float_le(1.5f,  0.5f));
}

/* ── Both negative ───────────────────────────────────────────────────────── */
void test_both_negative(void) {
    TEST_ASSERT_TRUE(float_le(-2.0f, -1.0f));   /* -2 <= -1 */
    TEST_ASSERT_TRUE(float_le(-1.0f, -1.0f));   /* equal */
    TEST_ASSERT_FALSE(float_le(-1.0f, -2.0f));
    TEST_ASSERT_TRUE(float_le(-1.5f, -0.5f));
    TEST_ASSERT_FALSE(float_le(-0.5f, -1.5f));
}

/* ── Mixed signs ─────────────────────────────────────────────────────────── */
void test_mixed_signs(void) {
    TEST_ASSERT_TRUE(float_le(-1.0f,  1.0f));   /* negative <= positive */
    TEST_ASSERT_FALSE(float_le(1.0f, -1.0f));
    TEST_ASSERT_TRUE(float_le(-0.1f,  0.1f));
    TEST_ASSERT_TRUE(float_le(-100.0f, 1.0f));
}

/* ── Zero vs nonzero ─────────────────────────────────────────────────────── */
void test_zero_vs_nonzero(void) {
    TEST_ASSERT_TRUE(float_le( 0.0f,  1.0f));
    TEST_ASSERT_TRUE(float_le(-0.0f,  1.0f));
    TEST_ASSERT_FALSE(float_le(1.0f,  0.0f));
    TEST_ASSERT_FALSE(float_le(1.0f, -0.0f));
    TEST_ASSERT_TRUE(float_le(-1.0f,  0.0f));
    TEST_ASSERT_TRUE(float_le(-1.0f, -0.0f));
    TEST_ASSERT_FALSE(float_le(0.0f, -1.0f));
}

/* ── Boundary: FLT_MAX and FLT_MIN ──────────────────────────────────────── */
void test_extreme_values(void) {
    TEST_ASSERT_TRUE(float_le(FLT_MAX,   FLT_MAX));
    TEST_ASSERT_TRUE(float_le(-FLT_MAX, -FLT_MAX));
    TEST_ASSERT_TRUE(float_le(-FLT_MAX,  FLT_MAX));
    TEST_ASSERT_FALSE(float_le(FLT_MAX, -FLT_MAX));
    TEST_ASSERT_TRUE(float_le(FLT_MIN,   FLT_MAX));
    TEST_ASSERT_TRUE(float_le(1.0f,      FLT_MAX));
    TEST_ASSERT_TRUE(float_le(-FLT_MAX,  1.0f));
}

/* ── Infinity ────────────────────────────────────────────────────────────── */
void test_infinity(void) {
    float pos_inf =  1.0f / 0.0f;
    float neg_inf = -1.0f / 0.0f;

    TEST_ASSERT_TRUE(float_le(pos_inf,  pos_inf));
    TEST_ASSERT_TRUE(float_le(neg_inf,  neg_inf));
    TEST_ASSERT_TRUE(float_le(neg_inf,  pos_inf));
    TEST_ASSERT_FALSE(float_le(pos_inf, neg_inf));
    TEST_ASSERT_TRUE(float_le(1.0f,     pos_inf));
    TEST_ASSERT_TRUE(float_le(neg_inf,  1.0f));
    TEST_ASSERT_FALSE(float_le(pos_inf, 1.0f));
    TEST_ASSERT_FALSE(float_le(1.0f,    neg_inf));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_zero_equality);
    RUN_TEST(test_both_positive);
    RUN_TEST(test_both_negative);
    RUN_TEST(test_mixed_signs);
    RUN_TEST(test_zero_vs_nonzero);
    RUN_TEST(test_extreme_values);
    RUN_TEST(test_infinity);

    UNITY_END();
}
