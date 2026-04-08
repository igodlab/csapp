/* Unity Test Framework - MIT License - ThrowTheSwitch.org
 * Minimal implementation for CSAPP project use. */
#include "unity.h"
#include <stdio.h>

/* ── Global state ─────────────────────────────────────────────────────────── */
unsigned int Unity_TestsRun     = 0;
unsigned int Unity_TestsFailed  = 0;
unsigned int Unity_TestsIgnored = 0;
const char  *Unity_CurrentTestName = "";
const char  *Unity_CurrentTestFile = "";
unsigned int Unity_CurrentTestLine = 0;
int          Unity_CurrentTestFailed = 0;

/* ── Core failure reporter ────────────────────────────────────────────────── */
void UnityFail(const char *msg, unsigned int line)
{
    Unity_CurrentTestFailed = 1;
    Unity_TestsFailed++;
    printf("  \033[31mFAIL\033[0m  %s\n"
           "         line %u: %s\n",
           Unity_CurrentTestName, line, msg ? msg : "(no message)");
}
