#include <assert.h>
#include <stdio.h>

int formA(int x, unsigned m, unsigned n, unsigned count) {
  assert(n >= m && "Error: n must be greater than m.");
  unsigned s = n - count;
  if (s < m) {
    return 0;
  } else {
    return (x << s) + formA(x, m, n, count + 1);
  }
}

int formB(int x, unsigned m, unsigned n) {
  assert(n >= n && "Error: n must be grater than m.");
  return (x << (n + 1)) - (x << m);
}

int main(void) {
  // `14 = [0...01110]` can be decomposed as `14 = 2^3 + 2^2 + 2^1` 
  // thus `x = 14 -> x = (1 << 3) + (1 << 2) + (1 << 1)`
  int x = 1; // represents bits with active weights, other bits are zero
  int n = 3; // represents the third bit (starting from subindex zero)
  int m = 1; // represents the first bit(subindex zero)
  printf("form A = %i\n", formA(x, m, n, 0));
  printf("form B = %i\n", formB(x, m, n));
  return 0;
}

