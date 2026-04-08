/*
 * test_prob2-61.c — Unity test suite for CSAPP 3e Problem 2.61
 */

#include "../../../vendor/unity/unity.h"
#include <limits.h>

int condA(int x);
int condB(int x);
int condC(int x);
int condD(int x);

void setUp(void)    {}
void tearDown(void) {}

/* ── A. Any bit of x equals 1 ───────────────────────────────────────────────
 * False only when x = 0x00000000 (all zeros).
 */
void test_A_nonzero_is_true(void) {
    TEST_ASSERT_TRUE(condA(1));
    TEST_ASSERT_TRUE(condA(-1));        /* 0xFFFFFFFF — all ones       */
    TEST_ASSERT_TRUE(condA(INT_MAX));   /* 0x7FFFFFFF                  */
    TEST_ASSERT_TRUE(condA(INT_MIN));   /* 0x80000000 — only MSB set   */
    TEST_ASSERT_TRUE(condA(0x00000001));
    TEST_ASSERT_TRUE(condA(0x80000000));
}

void test_A_zero_is_false(void) {
    TEST_ASSERT_FALSE(condA(0));
}

/* ── B. Any bit of x equals 0 ───────────────────────────────────────────────
 * False only when x = 0xFFFFFFFF (all ones, i.e. -1 in two's complement).
 */
void test_B_not_all_ones_is_true(void) {
    TEST_ASSERT_TRUE(condB(0));
    TEST_ASSERT_TRUE(condB(1));
    TEST_ASSERT_TRUE(condB(INT_MAX));   /* 0x7FFFFFFF — MSB is 0       */
    TEST_ASSERT_TRUE(condB(INT_MIN));   /* 0x80000000 — lower bits are 0 */
    TEST_ASSERT_TRUE(condB(0x7FFFFFFF));
}

void test_B_all_ones_is_false(void) {
    TEST_ASSERT_FALSE(condB(-1));       /* 0xFFFFFFFF                  */
}

/* ── C. Any bit in the least significant byte equals 1 ──────────────────────
 * False only when the low byte is 0x00.
 */
void test_C_nonzero_lsb_is_true(void) {
    TEST_ASSERT_TRUE(condC(0x000000FF));
    TEST_ASSERT_TRUE(condC(0x000000EF));
    TEST_ASSERT_TRUE(condC(0xFFFFFFFF));
    TEST_ASSERT_TRUE(condC(0x12345601));
    TEST_ASSERT_TRUE(condC(1));
}

void test_C_zero_lsb_is_false(void) {
    TEST_ASSERT_FALSE(condC(0x00000000));
    TEST_ASSERT_FALSE(condC(0x12345600));
    TEST_ASSERT_FALSE(condC(0xFFFFFF00));
}

/* ── D. Any bit in the most significant byte equals 0 ───────────────────────
 * False only when the high byte is 0xFF.
 */
void test_D_msb_not_all_ones_is_true(void) {
    TEST_ASSERT_TRUE(condD(0x00000000));
    TEST_ASSERT_TRUE(condD(0x7FFFFFFF));
    TEST_ASSERT_TRUE(condD(0x00FFFFFF));
    TEST_ASSERT_TRUE(condD(0x7F000000));
}

void test_D_msb_all_ones_is_false(void) {
    TEST_ASSERT_FALSE(condD(0xFF000000));
    TEST_ASSERT_FALSE(condD(0xFFFFFFFF));
    TEST_ASSERT_FALSE(condD(0xFF123456));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_A_nonzero_is_true);
    RUN_TEST(test_A_zero_is_false);

    RUN_TEST(test_B_not_all_ones_is_true);
    RUN_TEST(test_B_all_ones_is_false);

    RUN_TEST(test_C_nonzero_lsb_is_true);
    RUN_TEST(test_C_zero_lsb_is_false);

    RUN_TEST(test_D_msb_not_all_ones_is_true);
    RUN_TEST(test_D_msb_all_ones_is_false);

    UNITY_END();
}
