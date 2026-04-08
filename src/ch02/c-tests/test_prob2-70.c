/*
 * test_prob2-70.c — Unity test suite for CSAPP 3e Problem 2.70
 */

#include "../../../vendor/unity/unity.h"
#include <limits.h>

int fits_bits(int x, int n);

void setUp(void)    {}
void tearDown(void) {}

/* ── n=1: representable range is {-1, 0} ────────────────────────────────────*/
void test_n1(void) {
    TEST_ASSERT_TRUE(fits_bits(0,  1));
    TEST_ASSERT_TRUE(fits_bits(-1, 1));
    TEST_ASSERT_FALSE(fits_bits(1,  1));
    TEST_ASSERT_FALSE(fits_bits(-2, 1));
}

/* ── n=4: range [-8, 7] ──────────────────────────────────────────────────── */
void test_n4_within_range(void) {
    TEST_ASSERT_TRUE(fits_bits(0,   4));
    TEST_ASSERT_TRUE(fits_bits(7,   4));
    TEST_ASSERT_TRUE(fits_bits(-8,  4));
}

void test_n4_boundary_overflow(void) {
    TEST_ASSERT_FALSE(fits_bits(8,   4));
    TEST_ASSERT_FALSE(fits_bits(-9,  4));
}

/* ── n=8: range [-128, 127] ──────────────────────────────────────────────── */
void test_n8_within_range(void) {
    TEST_ASSERT_TRUE(fits_bits(0,    8));
    TEST_ASSERT_TRUE(fits_bits(127,  8));
    TEST_ASSERT_TRUE(fits_bits(-128, 8));
}

void test_n8_boundary_overflow(void) {
    TEST_ASSERT_FALSE(fits_bits(128,  8));
    TEST_ASSERT_FALSE(fits_bits(-129, 8));
}

/* ── n=16: range [-32768, 32767] ─────────────────────────────────────────── */
void test_n16_within_range(void) {
    TEST_ASSERT_TRUE(fits_bits(0,      16));
    TEST_ASSERT_TRUE(fits_bits(32767,  16));
    TEST_ASSERT_TRUE(fits_bits(-32768, 16));
}

void test_n16_boundary_overflow(void) {
    TEST_ASSERT_FALSE(fits_bits(32768,  16));
    TEST_ASSERT_FALSE(fits_bits(-32769, 16));
}

/* ── n=w (32): any int fits ──────────────────────────────────────────────── */
void test_n32_full_range(void) {
    TEST_ASSERT_TRUE(fits_bits(0,       32));
    TEST_ASSERT_TRUE(fits_bits(INT_MAX, 32));
    TEST_ASSERT_TRUE(fits_bits(INT_MIN, 32));
    TEST_ASSERT_TRUE(fits_bits(1,       32));
    TEST_ASSERT_TRUE(fits_bits(-1,      32));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_n1);

    RUN_TEST(test_n4_within_range);
    RUN_TEST(test_n4_boundary_overflow);

    RUN_TEST(test_n8_within_range);
    RUN_TEST(test_n8_boundary_overflow);

    RUN_TEST(test_n16_within_range);
    RUN_TEST(test_n16_boundary_overflow);

    RUN_TEST(test_n32_full_range);

    UNITY_END();
}
