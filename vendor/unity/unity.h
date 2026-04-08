/* Unity Test Framework - MIT License - ThrowTheSwitch.org
 * Minimal header for CSAPP project use. */
#ifndef UNITY_H
#define UNITY_H

#include "unity_internals.h"

/* ── Test runner macros ───────────────────────────────────────────────────── */

#define RUN_TEST(func)                                      \
    do {                                                    \
        Unity_CurrentTestName = #func;                      \
        Unity_CurrentTestFailed = 0;                        \
        Unity_TestsRun++;                                   \
        func();                                             \
        if (!Unity_CurrentTestFailed)                       \
            printf("  \033[32mPASS\033[0m  %s\n", #func);  \
    } while (0)

#define UNITY_BEGIN()                                           \
    do {                                                        \
        Unity_TestsRun    = 0;                                  \
        Unity_TestsFailed = 0;                                  \
        Unity_TestsIgnored= 0;                                  \
        printf("\033[1m--- Running: %s ---\033[0m\n", __FILE__);\
    } while (0)

#define UNITY_END()                                                         \
    do {                                                                    \
        printf("\033[1m%u Tests  %u Failures  %u Ignored\033[0m\n",        \
               Unity_TestsRun, Unity_TestsFailed, Unity_TestsIgnored);     \
        if (Unity_TestsFailed == 0)                                         \
            printf("\033[32mOK\033[0m\n");                                  \
        else                                                                \
            printf("\033[31mFAIL\033[0m\n");                                \
        return (int)Unity_TestsFailed;                                      \
    } while (0)

#endif /* UNITY_H */
