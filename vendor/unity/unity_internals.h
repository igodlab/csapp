/* Unity Test Framework - MIT License - ThrowTheSwitch.org
 * Minimal header for CSAPP project use. */
#ifndef UNITY_INTERNALS_H
#define UNITY_INTERNALS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* ── Counters ─────────────────────────────────────────────────────────────── */
extern unsigned int Unity_TestsRun;
extern unsigned int Unity_TestsFailed;
extern unsigned int Unity_TestsIgnored;
extern const char  *Unity_CurrentTestName;
extern const char  *Unity_CurrentTestFile;
extern unsigned int Unity_CurrentTestLine;
extern int          Unity_CurrentTestFailed;

/* ── Assertion core ───────────────────────────────────────────────────────── */
void UnityFail(const char *msg, unsigned int line);

#define UNITY_TEST_ASSERT(condition, line, msg)  \
    do { if (!(condition)) { UnityFail((msg), (line)); } } while (0)

/* ── Public assertion macros ─────────────────────────────────────────────── */
#define TEST_ASSERT_TRUE(cond) \
    UNITY_TEST_ASSERT((cond), __LINE__, #cond " is not true")

#define TEST_ASSERT_FALSE(cond) \
    UNITY_TEST_ASSERT(!(cond), __LINE__, #cond " is not false")

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UNITY_TEST_ASSERT((expected) == (actual), __LINE__, \
        "Expected " #expected " == " #actual)

#define TEST_ASSERT_EQUAL_DOUBLE(expected, actual) \
    UNITY_TEST_ASSERT((expected) == (actual), __LINE__, \
        "Expected " #expected " == " #actual)

#define TEST_PASS() do {} while (0)

#define TEST_FAIL_MESSAGE(msg) UnityFail((msg), __LINE__)

#endif /* UNITY_INTERNALS_H */
