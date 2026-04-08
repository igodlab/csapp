/*
 * test_prob2-63.c — Unity test suite for CSAPP 3e Problem 2.63
 */

#include "../../../vendor/unity/unity.h"

unsigned srl(unsigned x, int k);
int      sra(int x, int k);

void setUp(void)    {}
void tearDown(void) {}

/* ── srl: logical right shift ───────────────────────────────────────────────
 * Must fill vacated bits with 0, never with sign bit.
 */

void test_srl_identity(void) {
    TEST_ASSERT_EQUAL_INT(0x00000001, srl(0x00000001, 0));
    TEST_ASSERT_EQUAL_INT(0x89ABCDEF, srl(0x89ABCDEF, 0));
    TEST_ASSERT_EQUAL_INT(0x00000000, srl(0x00000000, 0));
}

void test_srl_positive_values(void) {
    TEST_ASSERT_EQUAL_INT(0x00000000, srl(0x00000001, 1));
    TEST_ASSERT_EQUAL_INT(0x00000000, srl(0x00000000, 4));
}

/* Critical: high bit set — must NOT sign-extend */
void test_srl_high_bit_set(void) {
    TEST_ASSERT_EQUAL_INT(0x40000000, srl(0x80000000, 1));
    TEST_ASSERT_EQUAL_INT(0x08000000, srl(0x80000000, 4));
    TEST_ASSERT_EQUAL_INT(0x7FFFFFFF, srl(0xFFFFFFFF, 1));
    TEST_ASSERT_EQUAL_INT(0x0FFFFFFF, srl(0xFFFFFFFF, 4));
    TEST_ASSERT_EQUAL_INT(0x00FFFFFF, srl(0xFFFFFFFF, 8));
}

/* Shift by w-1: only MSB survives, result must be 0 or 1 */
void test_srl_shift_by_w_minus_1(void) {
    TEST_ASSERT_EQUAL_INT(0x00000001, srl(0x80000000, 31));
    TEST_ASSERT_EQUAL_INT(0x00000000, srl(0x7FFFFFFF, 31));
    TEST_ASSERT_EQUAL_INT(0x00000001, srl(0xFFFFFFFF, 31));
}

/* ── sra: arithmetic right shift ────────────────────────────────────────────
 * Must fill vacated bits with the sign bit (0 for positive, 1 for negative).
 */

void test_sra_identity(void) {
    TEST_ASSERT_EQUAL_INT(0x00000001, sra(0x00000001, 0));
    TEST_ASSERT_EQUAL_INT(0x89ABCDEF, sra(0x89ABCDEF, 0));
    TEST_ASSERT_EQUAL_INT(0x00000000, sra(0x00000000, 0));
}

/* Positive values: MSB is 0, so vacated bits fill with 0 */
void test_sra_positive_values(void) {
    TEST_ASSERT_EQUAL_INT(0x3FFFFFFF, sra(0x7FFFFFFF, 1));
    TEST_ASSERT_EQUAL_INT(0x00000001, sra(0x00000008, 3));
    TEST_ASSERT_EQUAL_INT(0x00000000, sra(0x00000000, 4));
}

/* Negative values: must sign-extend with 1s */
void test_sra_negative_sign_extension(void) {
    TEST_ASSERT_EQUAL_INT(0xC0000000, sra(0x80000000, 1));
    TEST_ASSERT_EQUAL_INT(0xF8000000, sra(0x80000000, 4));
    TEST_ASSERT_EQUAL_INT(0xFFFFFFFF, sra(0xFFFFFFFF, 1));
    TEST_ASSERT_EQUAL_INT(0xFFFFFFFF, sra(0xFFFFFFFF, 4));
    TEST_ASSERT_EQUAL_INT(0xF8000000, sra(0x80000008, 4));
}

/* Shift by w-1: all 1s for negative, all 0s for positive */
void test_sra_shift_by_w_minus_1(void) {
    TEST_ASSERT_EQUAL_INT(0xFFFFFFFF, sra(0x80000000, 31));
    TEST_ASSERT_EQUAL_INT(0x00000000, sra(0x7FFFFFFF, 31));
    TEST_ASSERT_EQUAL_INT(0xFFFFFFFF, sra(0xFFFFFFFF, 31));
}

/* ── srl vs sra agreement on positive values ────────────────────────────────
 * For any x with MSB=0, both shifts must produce identical results.
 */
void test_srl_sra_agree_on_positives(void) {
    TEST_ASSERT_EQUAL_INT(srl(0x00000001, 1), (unsigned)sra(0x00000001, 1));
    TEST_ASSERT_EQUAL_INT(srl(0x7FFFFFFF, 4), (unsigned)sra(0x7FFFFFFF, 4));
    TEST_ASSERT_EQUAL_INT(srl(0x12345678, 8), (unsigned)sra(0x12345678, 8));
    TEST_ASSERT_EQUAL_INT(srl(0x00000000, 4), (unsigned)sra(0x00000000, 4));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_srl_identity);
    RUN_TEST(test_srl_positive_values);
    RUN_TEST(test_srl_high_bit_set);
    RUN_TEST(test_srl_shift_by_w_minus_1);

    RUN_TEST(test_sra_identity);
    RUN_TEST(test_sra_positive_values);
    RUN_TEST(test_sra_negative_sign_extension);
    RUN_TEST(test_sra_shift_by_w_minus_1);

    RUN_TEST(test_srl_sra_agree_on_positives);

    UNITY_END();
}
