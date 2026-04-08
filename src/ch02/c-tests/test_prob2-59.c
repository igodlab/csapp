/*
 * test_prob2-59.c — Unity test suite for CSAPP 3e Problem 2.59
 *
 * Build (from src/ch02/tests/):
 *   make test_prob2-59
 * or just:
 *   make
 */

#include "../../../vendor/unity/unity.h"

// forward declaration (instead of a header file)
unsigned int merge_lsb(unsigned int x, unsigned int y);

void setUp(void)    {}
void tearDown(void) {}

/* Textbook example */
void test_textbook_example(void) {
    TEST_ASSERT_EQUAL_INT(0x765432EF, merge_lsb(0x89ABCDEF, 0x76543210));
}

/* LSB of x is 0x00 — upper bytes of y pass through unchanged */
void test_lsb_zero(void) {
    TEST_ASSERT_EQUAL_INT(0x76543200, merge_lsb(0x00000000, 0x76543210));
}

/* LSB of x is 0xFF — upper bytes of y pass through unchanged */
void test_lsb_all_ones(void) {
    TEST_ASSERT_EQUAL_INT(0x765432FF, merge_lsb(0xFFFFFFFF, 0x76543210));
}

/* x and y identical — result equals both */
void test_same_operands(void) {
    TEST_ASSERT_EQUAL_INT(0x12345678, merge_lsb(0x12345678, 0x12345678));
}

/* Upper bytes of x are ignored entirely */
void test_upper_bytes_of_x_ignored(void) {
    TEST_ASSERT_EQUAL_INT(0xAABBCCEF, merge_lsb(0x000000EF, 0xAABBCC00));
}

/* Upper bytes of y are preserved exactly */
void test_upper_bytes_of_y_preserved(void) {
    TEST_ASSERT_EQUAL_INT(0xDEADBEAB, merge_lsb(0x000000AB, 0xDEADBE00));
}

/* Both operands zero */
void test_both_zero(void) {
    TEST_ASSERT_EQUAL_INT(0x00000000, merge_lsb(0x00000000, 0x00000000));
}

/* Both operands max */
void test_both_max(void) {
    TEST_ASSERT_EQUAL_INT(0xFFFFFFFF, merge_lsb(0xFFFFFFFF, 0xFFFFFFFF));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_textbook_example);
    RUN_TEST(test_lsb_zero);
    RUN_TEST(test_lsb_all_ones);
    RUN_TEST(test_same_operands);
    RUN_TEST(test_upper_bytes_of_x_ignored);
    RUN_TEST(test_upper_bytes_of_y_preserved);
    RUN_TEST(test_both_zero);
    RUN_TEST(test_both_max);

    UNITY_END();
}
