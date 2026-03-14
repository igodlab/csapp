#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++)
    printf(" %.2x", start[i]);
  printf("\n");
}

void show_int(int x) {
  printf("> size of `int` is %zu bytes, value is `int ival=`\n", sizeof(int));
  show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
  printf("> size of `float` is %zu bytes, value `float fval=`\n", sizeof(float));
  show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
  printf("> size of `void *` is %zu bytes (determined by 64-bit machine). Its value is the address `int *pval = &ival = `\n", sizeof(void *));
  show_bytes((byte_pointer) &x, sizeof(void *));
}

void show_value_at_pointer(int *ptr) {
  printf("> size of `int *` is %zu bytes. The pointer address is: %p\n", sizeof(int *), (void *)ptr);
  printf("  value at that address: %d_(decimal)\n", *ptr);
}

void show_pointer_char(const char *cval) {
  printf("> size of `const char *` is %zu bytes. The pointer address is %p\n", sizeof(const char *), (void *)cval);
  printf("  value at that address: %zu_(decimal)=0x61='a'\n", *cval);
}

void test_show_bytes(int val) {
  int ival = val;
  float fval = (float) ival;
  int *pval = &ival;
  show_int(ival);
  show_float(fval);
  show_pointer(pval);

  show_value_at_pointer(pval);

  const char *cval = "abcxyz"; // cval is a pointer to the first element of the 'abcxyz' string
  show_pointer_char(cval);
}

int main() {
  int value = 12345;
  test_show_bytes(value);
}
