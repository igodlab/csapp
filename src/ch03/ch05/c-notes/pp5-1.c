/*
The following problem illustrates the way memory aliasing can cause unexpected 
program behavior. Consider the following procedure to swap two values.

If this procedure is called with xp equal to yp. what effect will it have? 
*/

/* Swap value x at xp with value y at yp */
#include <stdio.h>
void swap(long *xp, long *yp)
{
  *xp = *xp + *yp; // x + y
  *yp = *xp - *yp; // x + y - y = x
  *xp = *xp - *yp; // x + y - x = y
}


int main(void) {
  long x, y;

  x = 1000;
  y = x;

  printf("If this procedure is called with xp equal to yp eg. x=y=%p\nThen memory alliasing gets triggered\n", (void *)&x);

  swap(&x, &y);
  printf("After swapping we get the new values\nx = %ld, y = %ld\n", x, y);
  return 0;
}
