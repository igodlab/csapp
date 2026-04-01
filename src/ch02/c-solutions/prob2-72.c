/*
2.72 ♦♦ 
You are given the task of writing a function that will copy an integer val into a 
buffer buf, but it should do so only if enough space is available in the buffer. 
Here is the code you write: 

```c
// Copy integer into buffer if space is available 
// WARNING The following code is buggy
void copy_int (int val,  void *buf, int maxbytes) { 
  if (maxbytes-sizeof(val)  >= 0) 
  memcpy(buf, (void *) &val, sizeof(val)); 
}
```

This code makes use of the library function memcpy. Although its use is a bit 
artificial here, where we simply want to copy an int, it illustrates an approach 
commonly used to copy larger data structures. 
You carefuJly test the code and discover that it always copies the value to the 
buffer, even when maxbytes is too small. 

A. Explain why the conditional test in the code always succeeds. Hint: The 
sizeof operator returns a value of type size_t. 
B. Show how you can rewrite the conditional test to make it work properly. 
*/

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int copy_int(int val, void *buf, int maxbytes) {
  // assert(maxbytes % 4 == 0 && "Error: maxbytes argument must be a multiple of a byte.");
  // PERF: `sizeof()` returns the size_t of its operand, but it treats the result as unsigned
  // -> thus the original conditional expression `if(maxbytes - sizeof(val) >= 0) {...}` 
  // is always true given C's promotion rules ie. if there is an unsigned operand then other variables are also treated as unsigned
  // So the fix is to compare values
  if (maxbytes - ((int) sizeof(val)) >= 0) {
    memcpy(buf, (void *) &val, sizeof(val));
    return 1;
  }
  return 0;
}

/* >>> TESTS >>> */
#define RUN(name) do {                      \
    name();                                 \
    printf("  PASS  %s\n", #name);         \
} while (0)

static void fill(void *buf, size_t n, unsigned char byte) {
    memset(buf, byte, n);
}

// various tests
void test_negative_maxbytes_does_not_copy(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xAB);

    int copied = copy_int(42, buf, -1);
    assert(copied == 0);
    for (size_t i = 0; i < sizeof(buf); i++)
        assert(buf[i] == 0xAB);
}

void test_zero_maxbytes_does_not_copy(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xCD);

    int copied = copy_int(99, buf, 0);
    assert(copied == 0);
    for (size_t i = 0; i < sizeof(buf); i++)
        assert(buf[i] == 0xCD);
}

void test_exact_space_copies(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0x00);
    int val = 0x12345678;

    int copied = copy_int(val, buf, 4); // 4 == sizeof(int), exact fit
    assert(copied == 1);

    int result;
    memcpy(&result, buf, sizeof(result));
    assert(result == val);
}

void test_ample_space_copies(void) {
    unsigned char buf[64];
    fill(buf, sizeof(buf), 0x00);
    int val = 0x1EADBEEF;

    int copied = copy_int(val, buf, 64);
    assert(copied == 1);

    int result;
    memcpy(&result, buf, sizeof(result));
    assert(result == val);
}

void test_copies_zero(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xFF);

    int copied = copy_int(0, buf, 8);
    assert(copied == 1);

    int result;
    memcpy(&result, buf, sizeof(result));
    assert(result == 0);
}

void test_copies_int_max(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0x00);

    int copied = copy_int(INT_MAX, buf, 8);
    assert(copied == 1);

    int result;
    memcpy(&result, buf, sizeof(result));
    assert(result == INT_MAX);
}

void test_copies_int_min(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0x00);

    int copied = copy_int(INT_MIN, buf, 8);
    assert(copied == 1);

    int result;
    memcpy(&result, buf, sizeof(result));
    assert(result == INT_MIN);
}

void test_no_buffer_overwrite_beyond_val(void) {
    unsigned char buf[32];
    fill(buf, sizeof(buf), 0xBB);

    copy_int(0x11223344, buf, 8);

    // only first sizeof(int)=4 bytes should be touched
    for (int i = (int)sizeof(int); i < (int)sizeof(buf); i++)
        assert(buf[i] == 0xBB);
}

void test_very_negative_maxbytes_does_not_copy(void) {
    unsigned char buf[16];
    fill(buf, sizeof(buf), 0xAA);

    int copied = copy_int(1, buf, INT_MIN);
    assert(copied == 0);
    for (size_t i = 0; i < sizeof(buf); i++)
        assert(buf[i] == 0xAA);
}

int main(void) {
    printf("Running copy_int tests...\n");
    RUN(test_negative_maxbytes_does_not_copy);
    RUN(test_zero_maxbytes_does_not_copy);
    RUN(test_exact_space_copies);
    RUN(test_ample_space_copies);
    RUN(test_copies_zero);
    RUN(test_copies_int_max);
    RUN(test_copies_int_min);
    RUN(test_no_buffer_overwrite_beyond_val);
    RUN(test_very_negative_maxbytes_does_not_copy);
    printf("\nAll tests passed.\n");
    return 0;
}
/* <<< TESTS <<< */
