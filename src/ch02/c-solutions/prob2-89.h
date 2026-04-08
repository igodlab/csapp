#ifndef PROB2_89_H
#define PROB2_89_H

/*
 * CSAPP 3e — Problem 2.89 public interface.
 *
 * Each condX function returns 1 if the expression holds for the given
 * inputs, 0 otherwise.
 */

int  condA(int x, double dx);
int  condB(int x, int y, double dx, double dy);
int  condC(double dx, double dy, double dz);
int  condD(double dx, double dy, double dz);
int  condE(double dx, double dz);

void prob289_print_bits(const char *name, double v);

#endif /* PROB2_89_H */
