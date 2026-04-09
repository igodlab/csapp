/*
 * test_prob2-72.c — Unity test suite for CSAPP 3e Problem 2.72
 */

#include "../../../vendor/unity/unity.h"
#include <limits.h>
#include <string.h>

int copy_int(int val, void *buf, int maxbytes);

void setUp(void)    {}
void tearDown(void) {}

static void fill(void *buf, size_t n, unsigned char byte) {
    memset(buf, byte, n);
}

/* ── Should NOT copy ─────────────────────────────────────────────────────── */
void test_negative_maxbytes_does_not_copy(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xAB);

    TEST_ASSERT_FALSE(copy_int(42, buf, -1));
    for (int i = 0; i < (int)sizeof(buf); i++)
        TEST_ASSERT_EQUAL_INT(0xAB, buf[i]);
}

void test_zero_maxbytes_does_not_copy(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xCD);

    TEST_ASSERT_FALSE(copy_int(99, buf, 0));
    for (int i = 0; i < (int)sizeof(buf); i++)
        TEST_ASSERT_EQUAL_INT(0xCD, buf[i]);
}

void test_very_negative_maxbytes_does_not_copy(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xAA);

    TEST_ASSERT_FALSE(copy_int(1, buf, INT_MIN));
    for (int i = 0; i < (int)sizeof(buf); i++)
        TEST_ASSERT_EQUAL_INT(0xAA, buf[i]);
}

/* ── Should copy ─────────────────────────────────────────────────────────── */
void test_exact_space_copies(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0x00);
    int val = 0x12345678;

    TEST_ASSERT_TRUE(copy_int(val, buf, 4));

    int result;
    memcpy(&result, buf, sizeof(result));
    TEST_ASSERT_EQUAL_INT(val, result);
}

void test_ample_space_copies(void) {
    unsigned char buf[64];
    fill(buf, sizeof(buf), 0x00);
    int val = 0x1EADBEEF;

    TEST_ASSERT_TRUE(copy_int(val, buf, 64));

    int result;
    memcpy(&result, buf, sizeof(result));
    TEST_ASSERT_EQUAL_INT(val, result);
}

void test_copies_zero(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xFF);

    TEST_ASSERT_TRUE(copy_int(0, buf, 8));

    int result;
    memcpy(&result, buf, sizeof(result));
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_copies_int_max(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0x00);

    TEST_ASSERT_TRUE(copy_int(INT_MAX, buf, 8));

    int result;
    memcpy(&result, buf, sizeof(result));
    TEST_ASSERT_EQUAL_INT(INT_MAX, result);
}

void test_copies_int_min(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0x00);

    TEST_ASSERT_TRUE(copy_int(INT_MIN, buf, 8));

    int result;
    memcpy(&result, buf, sizeof(result));
    TEST_ASSERT_EQUAL_INT(INT_MIN, result);
}

/* ── No buffer overwrite beyond sizeof(int) ─────────────────────────────── */
void test_no_buffer_overwrite_beyond_val(void) {
    unsigned char buf[32];
    fill(buf, sizeof(buf), 0xBB);

    copy_int(0x11223344, buf, 8);

    for (int i = (int)sizeof(int); i < (int)sizeof(buf); i++)
        TEST_ASSERT_EQUAL_INT(0xBB, buf[i]);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_negative_maxbytes_does_not_copy);
    RUN_TEST(test_zero_maxbytes_does_not_copy);
    RUN_TEST(test_very_negative_maxbytes_does_not_copy);

    RUN_TEST(test_exact_space_copies);
    RUN_TEST(test_ample_space_copies);
    RUN_TEST(test_copies_zero);
    RUN_TEST(test_copies_int_max);
    RUN_TEST(test_copies_int_min);

    RUN_TEST(test_no_buffer_overwrite_beyond_val);

    UNITY_END();
}
