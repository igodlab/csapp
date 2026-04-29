#include <stdio.h>

void twiddle1(long *xp, long *yp) {
  *xp += *yp;
  *xp += *yp;
}

void twiddle2(long *xp, long *yp) {
  *xp += 2 * *yp;
}

void opt_twiddle1(long *xp) {
  *xp += *xp;
  *xp += *xp;
}

int main(void) {
  long x1 = 4;
  long y1 = 3;

  long x2 = x1;
  long y2 = y1;

  long opt_x = x1;

  printf("For initial x = %ld, y = %ld\n", x1, y1);

  // test twiddle functions
  twiddle1(&x1, &y1);
  printf("\ntwiddle1\nx = %ld, y = %ld", x1, y1);

  twiddle1(&x2, &y2);
  printf("\ntwiddle2\nx = %ld, y = %ld", x1, y2);

  opt_twiddle1(&opt_x);
  printf("\nopt_twiddle1\nx = %ld", opt_x);

  return 0;
}
