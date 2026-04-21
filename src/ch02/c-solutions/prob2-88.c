/*
2.88 ♦♦ 
Consider the following two 9-bit floating-point representations based on the 
IEEE floating-point format. 

1. Format A 
  - There is 1 sign bit. 
  - There are k = 5 exponent bits. The exponent bias is 15. 
  - There are 11 = 3 fraction bits. 
2. Format B 
  - There is 1 sign bit. 
  - There are k = 4 exponent bits. The exponent bias is 7. 
  - There are 11 = 4 fraction bits. 

In the following table, you are given some bit patterns in format A. and your task is to convert them to the closest value in format B. If rounding is necessary 
you should round toward $+\inf$. In addition, give the values of numbers given by 
the format A and format B bit patterns. Give these as whole numbers (e.g., 17) or as fractions (e.g., 17/64 or 17/2^6). 
*/

#include <stdio.h>

void print_table(void) {
  // Column width 
  int w = 18;

  // Header
  printf("%-24s %-*s\n",
      "       Format A", w/2, "            Format B");

  // Header separator
  printf("%-12s %-*s\n",
      "-------------------------",
      w, " ------------------------");

  // Sub-header
    printf("%-12s %-*s %-12s %-*s\n",
           " Format A", w, " Value A", " Format B", w, " Value B");
    printf("%-12s %-*s %-12s %-*s\n",
           "------------", w, "--------------",
           "------------", w, "--------------");
 
#define ROW(fa, va, fb, vb) \
    printf("%-12s %-*s %-12s %-*s\n", fa, w, va, fb, w, vb)
 
    ROW("1 01111 001", " -9/8",      "1 0111 0010", " -9/8");
    ROW("0 10110 011", " 176",       "0 1110 0110", " 176");
    ROW("1 00111 010", " -5/256",    "1 0000 0101", " -5/256");
    ROW("0 00000 111", " 7/131072",  "0 0000 0000", " 0");
    ROW("1 11100 000", " -8192",     "1 1111 0000", " -inf");
    ROW("0 10111 100", " 384",       "0 1111 0000", " +inf");
 
#undef ROW
}

// int main(void) {
//   print_table();
//   return 0;
// }
