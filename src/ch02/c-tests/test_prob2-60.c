/*
 * test_prob2-60.c — Unity test suite for CSAPP 3e Problem 2.60
 */

#include "../../../vendor/unity/unity.h"

unsigned replace_byte(unsigned x, int i, unsigned char b);

void setUp(void)    {}
void tearDown(void) {}

/* ── Textbook examples ───────────────────────────────────────────────────── */
void test_textbook_examples(void) {
    TEST_ASSERT_EQUAL_INT(0x12AB5678, replace_byte(0x12345678, 2, 0xAB));
    TEST_ASSERT_EQUAL_INT(0x123456AB, replace_byte(0x12345678, 0, 0xAB));
}

/* ── Byte 0 (LSB) ────────────────────────────────────────────────────────── */
void test_byte0(void) {
    TEST_ASSERT_EQUAL_INT(0x000000FF, replace_byte(0x00000000, 0, 0xFF));
    TEST_ASSERT_EQUAL_INT(0xFFFFFF00, replace_byte(0xFFFFFFFF, 0, 0x00));
    TEST_ASSERT_EQUAL_INT(0x12345600, replace_byte(0x12345678, 0, 0x00));
}

/* ── Byte 1 ──────────────────────────────────────────────────────────────── */
void test_byte1(void) {
    TEST_ASSERT_EQUAL_INT(0x0000FF00, replace_byte(0x00000000, 1, 0xFF));
    TEST_ASSERT_EQUAL_INT(0xFFFF00FF, replace_byte(0xFFFFFFFF, 1, 0x00));
    TEST_ASSERT_EQUAL_INT(0x1234AB78, replace_byte(0x12345678, 1, 0xAB));
}

/* ── Byte 2 ──────────────────────────────────────────────────────────────── */
void test_byte2(void) {
    TEST_ASSERT_EQUAL_INT(0x00FF0000, replace_byte(0x00000000, 2, 0xFF));
    TEST_ASSERT_EQUAL_INT(0xFF00FFFF, replace_byte(0xFFFFFFFF, 2, 0x00));
    TEST_ASSERT_EQUAL_INT(0x12AB5678, replace_byte(0x12345678, 2, 0xAB));
}

/* ── Byte 3 (MSB) ────────────────────────────────────────────────────────── */
void test_byte3(void) {
    TEST_ASSERT_EQUAL_INT(0xFF000000, replace_byte(0x00000000, 3, 0xFF));
    TEST_ASSERT_EQUAL_INT(0x00FFFFFF, replace_byte(0xFFFFFFFF, 3, 0x00));
    TEST_ASSERT_EQUAL_INT(0xAB345678, replace_byte(0x12345678, 3, 0xAB));
}

/* ── Same byte value: result equals x ───────────────────────────────────── */
void test_replace_with_same_value(void) {
    TEST_ASSERT_EQUAL_INT(0x12345678, replace_byte(0x12345678, 0, 0x78));
    TEST_ASSERT_EQUAL_INT(0x12345678, replace_byte(0x12345678, 1, 0x56));
    TEST_ASSERT_EQUAL_INT(0x12345678, replace_byte(0x12345678, 2, 0x34));
    TEST_ASSERT_EQUAL_INT(0x12345678, replace_byte(0x12345678, 3, 0x12));
}

/* ── All zeros / all ones ────────────────────────────────────────────────── */
void test_all_zeros_and_ones(void) {
    TEST_ASSERT_EQUAL_INT(0x00000000, replace_byte(0x00000000, 0, 0x00));
    TEST_ASSERT_EQUAL_INT(0xFFFFFFFF, replace_byte(0xFFFFFFFF, 0, 0xFF));
    TEST_ASSERT_EQUAL_INT(0x00FF0000, replace_byte(0x00000000, 2, 0xFF));
    TEST_ASSERT_EQUAL_INT(0xFFFF00FF, replace_byte(0xFFFFFFFF, 1, 0x00));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_textbook_examples);
    RUN_TEST(test_byte0);
    RUN_TEST(test_byte1);
    RUN_TEST(test_byte2);
    RUN_TEST(test_byte3);
    RUN_TEST(test_replace_with_same_value);
    RUN_TEST(test_all_zeros_and_ones);

    UNITY_END();
}
