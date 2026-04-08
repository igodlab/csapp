#ifndef PROB2_59_H
#define PROB2_59_H

/*
 * CSAPP American 3ed — Problem 2.59 public interface.
 *
 * Condition is met if LSB of x replaces LSB of y while keeping the rest 
 * upper bytes of y.
*/

unsigned int merge_lsb(unsigned int x, unsigned int y);

#endif /* PROB2_59_H */
