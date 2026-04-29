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
  long x, y, xy;
  long *xp = &x;
  long *yp = &y;
  long *xyp = &x;

  x = 1000;
  y = 3000;
  printf("Initial values x = %ld, y = %ld\n\n", x, y);

  // No memory aliasing case
  printf("If this procedure is called with: xp=%p, yp=%p\n", (void *)xp, (void *)yp);
  swap(xp, yp);
  printf("    -> No aliasing occurs, thus we have a succesful swap x=%ld, y=%ld\n", x, y);

  // Memory aliasing case
  printf("\nHowever if the procedure is called with xp=yp=%p\n", (void *)xyp);
  swap(xp, xyp);
  printf("    -> aliasing occurs! Yielding x = %ld, y = %ld\n", x, y);
  return 0;
}
