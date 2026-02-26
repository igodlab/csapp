#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
  size_t i;
  for (i = 0; i < len; i++)
    printf(" %.2x", start[i]);
  printf("\n");
}

void show_int(int x) {
  printf("> size of `int` is %zu bytes:\n", sizeof(int));
  show_bytes((byte_pointer) &x, sizeof(int));
}

void show_float(float x) {
  printf("> size of `float` is %zu bytes:\n", sizeof(float));
  show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
  printf("> size of `void *` is %zu bytes:\n", sizeof(void *));
  show_bytes((byte_pointer) &x, sizeof(void *));
}

void show_value_at_pointer(int *ptr) {
  printf("Address stored in pointer: %p\n", (void *)ptr);
  printf("Value at that address: %d\n", *ptr);
}

void test_show_bytes(int val) {
  int ival = val;
  float fval = (float) ival;
  int *pval = &ival;
  show_int(ival);
  show_float(fval);
  show_pointer(pval);

  show_value_at_pointer(pval);
}

int main() {
  int value = 12345;
  test_show_bytes(value);
}
