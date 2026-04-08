## Chapter 2 - Representing and Manipulating Information

### 2.2 Integer Representations

- Example `casting.c`
    - Prints on screen various types and pointers printed in hexadecimal. These hex values are the actual bits that the machine stores.
    - For instance `fval` is the value `int ival = 12345` casted to float ie. `float fval = (float) ival` so it is `12345.0`. 
        - when we run `./build/casting` we get the value `00 e4 40 46` printed.
        - because we are on a little endian x86_64 machine the actual bits are: `0x4640e4 = 0b01000110010000001110010000000000`
        - just to workout a full verification we can convert the fval bits into **IEEE-754 32-bit single precision floating point format** 
