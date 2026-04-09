/*
2.87 ♦ 
The 2008 version of the IEEE floating-point standard, named IEEE 754-2008, 
includes a 16-bit "half-precision" floating-point format. It was originally devised 
by computer graphics companies for storing data in which a higher dynamic range 
is required than can be achieved with 16-bit integers. This format has 1 sign 
bit, 5 exponent bits (k = 5), and 10 fraction bits (n = 10). The exponent bias is 
2^(5-1) - 1 = 15.

Fill in the table that follows for each of the numbers given, with the following 
instructions for each column: 

Hex: The four hexadecimal digits describing the encoded form
M:  The value if the significand. This should be a number of the form x or x/y where x 
    is an integer and y is an integral power of 2. Examples include 0, 67/64 and 1/256
E: The integer value of the exponent
V: The numeric value represented. Use the notation x or x\times 2^z, where x and z are integers
D: The (possibly approximate) numerical value, as is printed using the %f formatting specification of printf

As an example, to represent the number 7/8 we would have s=0, M=3/4, and E=-1. Our number would therefore 
have an exponent field of `01110` (decimal value 15-1=14) and a significand field of `1100000000`, giving a 
hex representation `0x3B00`. The numerical value is 0.875. 
You need not fill in entries marked -----. 

Description           Hex          M            E            V            D
--------------------  -----------  -----------  -----------  -----------  -----------
-0                    --------     --------     --------     -0           -0.0
Smallest value > 2    --------     --------     --------     --------     --------
512                   --------     --------     --------     512          512.0
Largest denormalized  --------     --------     --------     --------     --------
-inf                  --------     --           --           -inf         -inf
Number with hex       3BB0         --------     --------     --------     --------
 representation 3BB0

*/

#include <stdio.h>

void print_table(void) {
    // Column width
    int w = 12;

    // Header
    printf("%-20s %-*s %-*s %-*s %-*s %-*s\n",
           "Description", w, " Hex", w, " M", w, " E", w, " V", w, " D");

    // Separator
    printf("%-20s %-*s %-*s %-*s %-*s %-*s\n",
           "--------------------",
           w, " -----------",
           w, " -----------",
           w, " -----------",
           w, " -----------",
           w, " -----------");

    // Row macro for convenience
    #define ROW(desc, hex, m, e, v, d) \
        printf("%-20s %-*s %-*s %-*s %-*s %-*s\n", desc, w, hex, w, m, w, e, w, v, w, d)

    ROW("-0",                  " 8000", " 0"        , " -14", " -0"          , " -0.0"    );
    ROW("Smallest value > 2",  " 4001", " 1025/1024", " 1"  , " 1025x2^(-9)" , " 2.00195" );
    ROW("512",                 " 6400", " 1"        , " 9"  , " 512"         , " 512.0"   );
    ROW("Largest denormalized"," 03FF", " 1023/1024", " -14", " 1023x2^(-24)", "0.000061" );
    ROW("-inf",                " FC00", " ---------", " ---", " -inf"        , " -inf"    );
    ROW("Number with hex",     " 3BB0", " 1968/1024", " -1" , " 123x2^(-7)"  , " 0.961"   );
    ROW(" representation 3BB0"," ",     " "         , " "   , " "            , " "        );
#undef ROW
}
