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
#include <string.h>

int copy_int(int val, void *buf, int maxbytes) {
  // assert(maxbytes % 4 == 0 && "Error: maxbytes argument must be a multiple of a byte.");
  // PERF:
  // A. `sizeof()` returns the size_t of its operand, but it treats the result as unsigned
  //    -> thus the original conditional expression `if(maxbytes - sizeof(val) >= 0) {...}` 
  //    is always true given C's promotion rules ie. if there is an unsigned operand then other variables are also treated as unsigned
  // B. The fix is to cast sizeof(val) to int so the comparison stays in signed arithmetic.
  if (maxbytes - ((int) sizeof(val)) >= 0) {
    memcpy(buf, (void *) &val, sizeof(val));
    return 1;
  }
  return 0;
}
