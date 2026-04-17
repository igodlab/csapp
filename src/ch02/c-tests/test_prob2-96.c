/*
 * test_prob2-96.c — Unity test suite for CSAPP 3e Problem 2.96
 *
 * float_f2i computes (int)f using bit manipulation only.
 * Returns 0x80000000 for out-of-range values.
 * Truncates toward zero (same as C's (int) cast).
 * NaN and infinity cases are not tested here.
 */

#include "../../../vendor/unity/unity.h"
#include <limits.h>
#include <stdint.h>

typedef union {
    float    f;
    uint32_t u;
} float_bits;

int float_f2i(float_bits f);

float_bits fb(float x) {
    float_bits r;
    r.f = x;
    return r;
}

float_bits fbu(uint32_t bits) {
    float_bits r;
    r.u = bits;
    return r;
}

void setUp(void)    {}
void tearDown(void) {}

/* ── Zero ────────────────────────────────────────────────────────────────── */
void test_positive_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fb(0.0f)));
}

void test_negative_zero(void) {
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fb(-0.0f)));
}

/* ── Denormalized numbers: all truncate to 0 ─────────────────────────────── */
void test_denorm_small_positive(void) {
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fbu(0x00000001)));
}

void test_denorm_largest(void) {
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fbu(0x007FFFFF)));
}

void test_denorm_negative(void) {
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fbu(0x80000001)));
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fbu(0x807FFFFF)));
}

/* ── Out of range: return 0x80000000 ─────────────────────────────────────── */
void test_overflow_large_positive(void) {
    TEST_ASSERT_EQUAL_INT(0x80000000, float_f2i(fb(2147483648.0f)));
}

void test_overflow_large_negative(void) {
    TEST_ASSERT_EQUAL_INT(0x80000000, float_f2i(fb(-2147483904.0f)));
}

void test_overflow_flt_max(void) {
    TEST_ASSERT_EQUAL_INT(0x80000000, float_f2i(fbu(0x7F7FFFFF)));
}

/* ── Exact integers: positive ────────────────────────────────────────────── */
void test_exact_one(void) {
    TEST_ASSERT_EQUAL_INT(1, float_f2i(fb(1.0f)));
}

void test_exact_two(void) {
    TEST_ASSERT_EQUAL_INT(2, float_f2i(fb(2.0f)));
}

void test_exact_powers_of_two(void) {
    TEST_ASSERT_EQUAL_INT(4,    float_f2i(fb(4.0f)));
    TEST_ASSERT_EQUAL_INT(256,  float_f2i(fb(256.0f)));
    TEST_ASSERT_EQUAL_INT(1024, float_f2i(fb(1024.0f)));
}

void test_exact_int_max_representable(void) {
    TEST_ASSERT_EQUAL_INT(1 << 30, float_f2i(fb((float)(1 << 30))));
}

/* ── Exact integers: negative ────────────────────────────────────────────── */
void test_exact_negative_one(void) {
    TEST_ASSERT_EQUAL_INT(-1, float_f2i(fb(-1.0f)));
}

void test_exact_negative_powers_of_two(void) {
    TEST_ASSERT_EQUAL_INT(-4,   float_f2i(fb(-4.0f)));
    TEST_ASSERT_EQUAL_INT(-256, float_f2i(fb(-256.0f)));
}

/* ── Truncation toward zero ──────────────────────────────────────────────── */
void test_truncate_positive_fraction(void) {
    TEST_ASSERT_EQUAL_INT(1, float_f2i(fb(1.5f)));
    TEST_ASSERT_EQUAL_INT(1, float_f2i(fb(1.9f)));
    TEST_ASSERT_EQUAL_INT(3, float_f2i(fb(3.7f)));
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fb(0.9f)));
    TEST_ASSERT_EQUAL_INT(0, float_f2i(fb(0.5f)));
}

void test_truncate_negative_fraction(void) {
    TEST_ASSERT_EQUAL_INT(-1, float_f2i(fb(-1.5f)));
    TEST_ASSERT_EQUAL_INT(-1, float_f2i(fb(-1.9f)));
    TEST_ASSERT_EQUAL_INT(-3, float_f2i(fb(-3.7f)));
    TEST_ASSERT_EQUAL_INT( 0, float_f2i(fb(-0.9f)));
    TEST_ASSERT_EQUAL_INT( 0, float_f2i(fb(-0.5f)));
}

/* ── INT_MIN boundary ────────────────────────────────────────────────────── */
void test_int_min_exactly(void) {
    TEST_ASSERT_EQUAL_INT(INT_MIN, float_f2i(fb(-2147483648.0f)));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_positive_zero);
    RUN_TEST(test_negative_zero);

    RUN_TEST(test_denorm_small_positive);
    RUN_TEST(test_denorm_largest);
    RUN_TEST(test_denorm_negative);

    RUN_TEST(test_overflow_large_positive);
    RUN_TEST(test_overflow_large_negative);
    RUN_TEST(test_overflow_flt_max);

    RUN_TEST(test_exact_one);
    RUN_TEST(test_exact_two);
    RUN_TEST(test_exact_powers_of_two);
    RUN_TEST(test_exact_int_max_representable);

    RUN_TEST(test_exact_negative_one);
    RUN_TEST(test_exact_negative_powers_of_two);

    RUN_TEST(test_truncate_positive_fraction);
    RUN_TEST(test_truncate_negative_fraction);

    RUN_TEST(test_int_min_exactly);

    UNITY_END();
}
