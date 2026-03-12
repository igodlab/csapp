#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    int32_t tmax32 = 2147483647;
    int32_t tmin32 = -tmax32 - 1; // as defined in DATA:MIN limits.h; #define INT_MAX 2147483647; #define INT_MIN (-INT_MAX - 1) 
    uint32_t tmax32u = 2147483647u;
    uint32_t tmin32u = 2147483648u;

    uint32_t umax32 = 4294967295;
    uint32_t umin32 = 0;

    // recreate some examples in Figure 2.19
    printf("%d  > -2147483647-1 = %d\n", tmax32, tmax32 > tmin32);
    printf("%du > -2147483647-1 = %d\n", tmax32u, tmax32u > tmin32);

    // cast unsigned to int before comparing operands
    printf("%d  > (int32_t) %uu = %d\n", tmax32, tmin32, tmax32 > ((int32_t) tmin32u));
    
    // Or using PRId32/PRIu32 format specifiers:
    // printf("%" PRId32 " > (int) %" PRIu32 " = %" PRId32 "\n", a, b, c);
    
    return 0;
}
