/*
2.89 ♦ 
We are running programs on a machine where values of type int have a 32-
bit two's-complement representation. Values of type f l oa t  use the 32-bit IEEE 
format, and values of type d oubl e use the 64-bit IEEE format. 
We generate arbitrary integer values x. y. and z, and convert them to values 
of type doubl e as follows: 

```c
// Create some arbitrary values
int x = random();
int y = random();
int z = random();

// Convert to double
double dx = (double) x;
double dy = (double) y;
double dz = (double) z;
```

For each of the following C expressions. you are to indicate whether or not the 
expression always yields 1. If it always yields 1, describe the underlying 
mathematical principles. Otherwise. give an example of arguments that make 
it yield 0. Note that you cannot use an IA32 machine running GCC to test your 
answers, since it would use the 80-bit extended-preci ion representation for both 
float  and double.

A. (float) x == (float) dx 
B. dx - dy == ( double) (x-y) 
C. (dx + dy) + dz == dx + (dy + dz) 
D. (dx * dy) * dz == dx * (dy * dz) 
E. dx / dx == dz / dz
*/

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* >>> Ansi color helpers >>> */
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define CYAN  "\033[36m"
#define BOLD  "\033[1m"
#define DIM   "\033[2m"
#define RST   "\033[0m"
/* <<< Ansi color helpers <<< */

/* ── Test infrastructure ─────────────────────────────────────────────────── */
static int g_pass = 0, g_fail = 0;

/*
 * expect(result, expected, label, note)
 *   result   : actual boolean produced by the expression under test
 *   expected : 1 → expression should be true, 0 → should be false (counter-ex)
 *   label    : short name printed in the report
 *   note     : one-line explanation of *why* this case is interesting
 */
static void expect(int result, int expected,
    const char *label, const char *note)
{
  int ok = (result == expected);
  if (ok) {
    printf("  " GREEN "PASS" RST "  %-46s " DIM "%s" RST "\n", label, note);
    g_pass++;
  } else {
    printf("  " RED "FAIL" RST "  %-46s " DIM "%s" RST "\n", label, note);
    printf("         got %d, expected %d\n", result, expected);
    g_fail++;
  }
}


// NOTE: The five conditions (volatile prevents the compiler from collapsing   ── */
// both sides into the same subexpression before comparing them)         ── */
static int condA(int x, double dx) {
  return (float)x == (float)dx;
}

static int condB(int x, int y, double dx, double dy) {
  /*
   * RHS casts (x-y) to double AFTER integer subtraction.
   * When y = INT_MIN, x-y overflows (UB, but wraps on x86 GCC), giving a
   * wrong integer that becomes a wrong double → differs from dx-dy.
   */
  volatile double lhs = dx - dy;
  volatile double rhs = (double)(x - y);
  return lhs == rhs;
}

static int condC(double dx, double dy, double dz) {
  /*
   * Exact because every int32 is exactly representable as double (fits in 53
   * bits), and the sum of any two int32 values fits in 33 bits, still $\ll 2^53$.
   * So no rounding ever occurs → associativity holds trivially.
   */
  volatile double lhs = (dx + dy) + dz;
  volatile double rhs = dx + (dy + dz);
  return lhs == rhs;
}

static int condD(double dx, double dy, double dz) {
  /*
   * Products of two int32 values can be $2^{31}2^{31}\sim 62$ bits wide. A double only holds
   * 53 bits of significand (implicit 1.M and 52 mantissa bits), so the product must be rounded.  
   * The two orderings round at different points → different final results.
   *
   * This REQUIRES strict IEEE 64-bit mode (see compile flags above).
   * With x87 80-bit registers GCC keeps extra precision and the difference
   * vanishes — that is what "Not detected with Linux/GCC" means.
   */
  volatile double lhs = (dx * dy) * dz;
  volatile double rhs = dx * (dy * dz);
  return lhs == rhs;
}

static int condE(double dx, double dz) {
  /*
   * IEEE 754: 0/0 = NaN, and NaN != NaN by definition.
   * So if either dx or dz is 0, the corresponding quotient is NaN and the
   * equality is false.
   */
  volatile double lhs = dx / dx;
  volatile double rhs = dz / dz;
  return lhs == rhs;
}

// Helper: hex dump of a double's bit pattern 
static void print_bits(const char *name, double v) {
  uint64_t bits;
  memcpy(&bits, &v, 8);
  printf("         %s = %.17g  (0x%016llx)\n",
      name, v, (unsigned long long)bits);
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SECTION A  —  (float)x == (float)dx                      ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * Converting int→double is always exact (53-bit mantissa covers all 32-bit
 * ints).  Converting to float rounds identically from both int and double
 * because the starting value is identical bit-for-bit.
 */
static void test_A(void) {
  printf(BOLD CYAN "\nA. (float)x == (float)dx" RST
      "  [always true]\n");

  int cases[][1] = {{0},{1},{-1},{INT_MAX},{INT_MIN},{12345678},{-987654321}};
  const char *names[] = {"0","1","-1","INT_MAX","INT_MIN","12345678","-987654321"};

  for (int i = 0; i < 7; i++) {
    int x   = cases[i][0];
    double dx = (double)x;
    char lbl[64];
    snprintf(lbl, sizeof lbl, "condA(x=%s)", names[i]);
    expect(condA(x, dx), 1, lbl,
        "same rounding path: int→float == double→float");
  }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SECTION B  —  dx - dy == (double)(x-y)                   NOT ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * Failure: y = INT_MIN.  Then x-y overflows signed int (UB; wraps on x86)
 * giving x + 2^31 as an int.  Cast to double that wrong bit pattern differs
 * from the correct dx - dy computed in double arithmetic.
 *
 * Non-overflow cases are always true because int subtraction that doesn't
 * overflow stays within [INT_MIN, INT_MAX] ⊂ exactly-representable doubles.
 */
static void test_B(void) {
  printf(BOLD CYAN "\nB. dx - dy == (double)(x-y)" RST
      "  [fails when y=INT_MIN]\n");

  /* Should be TRUE — no overflow */
  struct { int x, y; const char *lbl; } ok_cases[] = {
    {0,           0,            "x=0, y=0"},
    {INT_MAX,     0,            "x=INT_MAX, y=0"},
    {0,           INT_MAX,      "x=0, y=INT_MAX"},
    {INT_MIN,     0,            "x=INT_MIN, y=0"},
    {INT_MAX,     INT_MAX,      "x=y=INT_MAX"},
    {INT_MIN+1,   INT_MIN+1,    "x=y=INT_MIN+1"},
    {100,         -100,         "x=100, y=-100  (no overflow)"},
  };
  for (int i = 0; i < 7; i++) {
    int x = ok_cases[i].x, y = ok_cases[i].y;
    char lbl[64]; snprintf(lbl, sizeof lbl, "condB(%s)", ok_cases[i].lbl);
    expect(condB(x, y, (double)x, (double)y), 1, lbl,
        "no overflow → exact double subtraction");
  }

  /* Should be FALSE — overflow */
  printf("\n  --- overflow cases (expect FAIL = return 0) ---\n");

  /*
   * Manual's counter-example: x=0, y=INT_MIN
   *   x - y  in int: 0 - (-2147483648) overflows → wraps to -2147483648
   *   (double)(x-y) = -2147483648.0
   *   dx - dy        = 2147483648.0   ← correct, different!
   */
  {
    int x = 0, y = INT_MIN;
    double dx = (double)x, dy = (double)y;
    printf("\n  Diagnostic for x=0, y=INT_MIN:\n");
    print_bits("dx - dy        ", dx - dy);
    print_bits("(double)(x-y)  ", (double)(x - y));
    expect(condB(x, y, dx, dy), 0, "condB(x=0, y=INT_MIN)",
        "manual counter-example: int overflow wraps y subtraction");
  }
  /* Extra: x=1, y=INT_MIN — same overflow mechanism */
  {
    int x = 1, y = INT_MIN;
    double dx = (double)x, dy = (double)y;
    expect(condB(x, y, dx, dy), 0, "condB(x=1, y=INT_MIN)",
        "1 - INT_MIN overflows → wrong integer cast");
  }
  /* Extra: x=INT_MAX, y=INT_MIN — maximum possible overflow magnitude */
  {
    int x = INT_MAX, y = INT_MIN;
    double dx = (double)x, dy = (double)y;
    expect(condB(x, y, dx, dy), 0, "condB(x=INT_MAX, y=INT_MIN)",
        "largest overflow: INT_MAX - INT_MIN wraps to -1");
  }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SECTION C  —  (dx+dy)+dz == dx+(dy+dz)                   ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * Every int32 is exactly representable in double (fits in 53 bits).
 * The maximum possible sum of two int32 values is
 *   INT_MAX + INT_MAX = 2*2147483647 = 4294967294  < 2^32  ≪  2^53
 * so every partial sum in any grouping is also exactly representable.
 * No rounding ever occurs → both sides are identical bit-for-bit.
 */
static void test_C(void) {
  printf(BOLD CYAN "\nC. (dx+dy)+dz == dx+(dy+dz)" RST
      "  [always true]\n");

  struct { int x,y,z; const char *lbl; } cases[] = {
    {0,           0,         0,        "all zero"},
    {INT_MAX,     INT_MAX,   INT_MAX,  "all INT_MAX"},
    {INT_MIN,     INT_MIN,   INT_MIN,  "all INT_MIN"},
    {INT_MAX,     INT_MIN,   0,        "INT_MAX+INT_MIN+0"},
    {INT_MAX,     -1,        INT_MIN,  "INT_MAX,-1,INT_MIN"},
    {100000000,   200000000, 300000000,"large positives"},
    {-100000000, -200000000,-300000000,"large negatives"},
  };
  for (int i = 0; i < 7; i++) {
    double dx=(double)cases[i].x, dy=(double)cases[i].y, dz=(double)cases[i].z;
    char lbl[64]; snprintf(lbl,sizeof lbl,"condC(%s)",cases[i].lbl);
    expect(condC(dx,dy,dz),1,lbl,
        "all int32 sums < 2^33 << 2^53 → exact, no rounding");
  }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SECTION D  —  (dx*dy)*dz == dx*(dy*dz)                   NOT ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * Products of two int32 values can require up to 62 bits, but double only
 * stores 53.  The rounded intermediate differs between the two groupings.
 *
 * *** REQUIRES STRICT IEEE FLAGS — see file header ***
 * Without -mfpmath=sse -fno-associative-math -fno-fma the 80-bit x87
 * extended precision absorbs the rounding difference and both sides compare
 * equal, so the FAIL cases below will spuriously PASS.
 */
static void test_D(void) {
  printf(BOLD CYAN "\nD. (dx*dy)*dz == dx*(dy*dz)" RST
      "  [fails with large products]\n");

  /* Should be TRUE — small values, products fit in 53 bits */
  struct { int x,y,z; const char *lbl; } ok_cases[] = {
    {0,      0,      0,     "all zero"},
    {1,      1,      1,     "all one"},
    {-1,     1,      -1,    "signs only"},
    {1000,   1000,   1000,  "1e3 each, product 1e9 < 2^30 << 2^53"},
    {100000, 100000, 2,     "1e5,1e5,2  product 2e12 < 2^41 << 2^53"},
  };
  for (int i = 0; i < 5; i++) {
    double dx=(double)ok_cases[i].x,
           dy=(double)ok_cases[i].y,
           dz=(double)ok_cases[i].z;
    char lbl[64]; snprintf(lbl,sizeof lbl,"condD(%s)",ok_cases[i].lbl);
    expect(condD(dx,dy,dz),1,lbl,
        "products fit in 53 bits → exact → associativity holds");
  }

  /* Should be FALSE — verified counterexamples (brute-force searched) */
  printf("\n  --- verified counterexamples (brute-force found, diverge on x86-64) ---\n");
  printf("  NOTE: manual's TMax/TMax-1/TMax-2 do NOT diverge on x86-64 GCC.\n\n");

  /*
   * Counterexample 1 (brute-force found, verified to diverge on x86-64):
   *   x=-348573226, y=300159921, z=-998530546
   *
   *   (dx*dy)*dz → bits 0x45559ace442d6b74
   *   dx*(dy*dz) → bits 0x45559ace442d6b73   ← differ in 1 ULP
   *
   * The ~57-bit product dx*dy must be rounded to 53 bits.  Multiplying that
   * already-rounded value by dz rounds again.  The alternative grouping
   * first rounds dy*dz (a different ~57-bit product), then multiplies — the
   * two rounding decisions compound differently.
   */
  {
    int xi = -348573226, yi = 300159921, zi = -998530546;
    double dx = (double)xi, dy = (double)yi, dz = (double)zi;
    volatile double lhs = (dx * dy) * dz;
    volatile double rhs = dx * (dy * dz);
    printf("  Diagnostic: x=%d, y=%d, z=%d\n", xi, yi, zi);
    print_bits("(dx*dy)*dz", lhs);
    print_bits("dx*(dy*dz)", rhs);
    expect(condD(dx, dy, dz), 0,
        "condD(-348573226, 300159921, -998530546)",
        "verified: LSB differs — two rounding paths diverge by 1 ULP");
  }

  /*
   * Counterexample 2 (all-positive, large magnitudes):
   *   x=2055324957, y=1819187404, z=293595697
   *   bits differ by 1 ULP in the same way.
   */
  {
    int xi = 2055324957, yi = 1819187404, zi = 293595697;
    double dx = (double)xi, dy = (double)yi, dz = (double)zi;
    expect(condD(dx, dy, dz), 0,
        "condD(2055324957, 1819187404, 293595697)",
        "verified: all-positive large magnitudes, 1 ULP divergence");
  }

  /*
   * Counterexample 3 (all-negative intermediate):
   *   x=-291539692, y=-1606651893, z=-1898132382
   */
  {
    int xi = -291539692, yi = -1606651893, zi = -1898132382;
    double dx = (double)xi, dy = (double)yi, dz = (double)zi;
    expect(condD(dx, dy, dz), 0,
        "condD(-291539692, -1606651893, -1898132382)",
        "verified: all-negative triple, rounding diverges in LSB");
  }

  /* Reference: show the manual's values and why they DON'T diverge here */
  printf("\n  --- manual's TMax/TMax-1/TMax-2: reference only ---\n");
  {
    double dx = (double)INT_MAX;
    double dy = (double)(INT_MAX - 1);
    double dz = (double)(INT_MAX - 2);
    volatile double lhs = (dx * dy) * dz;
    volatile double rhs = dx * (dy * dz);
    print_bits("(dx*dy)*dz", lhs);
    print_bits("dx*(dy*dz)", rhs);
    printf("         equal? %d  ← same bits on x86-64 GCC"
        " (manual: 'not detected with Linux/GCC')\n", lhs == rhs);
  }
}

/* ═══════════════════════════════════════════════════════════════════════════
 * SECTION E  —  dx/dx == dz/dz                             NOT ALWAYS TRUE
 * ═══════════════════════════════════════════════════════════════════════════
 *
 * IEEE 754: 0.0/0.0 = NaN, and NaN != NaN (even itself).
 * So if x=0, dx/dx = NaN; if z≠0, dz/dz = 1.0 → NaN != 1.0 → false.
 * Also if both are 0: NaN != NaN → still false.
 *
 * For any nonzero finite double: d/d = 1.0 exactly → equality holds.
 */
static void test_E(void) {
  printf(BOLD CYAN "\nE. dx/dx == dz/dz" RST
      "  [fails when either operand is 0]\n");

  /* Should be TRUE — both nonzero */
  struct { int x, z; const char *lbl; } ok_cases[] = {
    {1,       1,        "x=1,   z=1"},
    {1,       2,        "x=1,   z=2"},
    {INT_MAX, INT_MIN,  "x=INT_MAX, z=INT_MIN"},
    {-1,      1,        "x=-1,  z=1"},
    {100,     -999,     "x=100, z=-999"},
  };
  for (int i = 0; i < 5; i++) {
    double dx = (double)ok_cases[i].x;
    double dz = (double)ok_cases[i].z;
    char lbl[64]; snprintf(lbl,sizeof lbl,"condE(%s)",ok_cases[i].lbl);
    expect(condE(dx, dz), 1, lbl,
        "nonzero/nonzero = 1.0 = 1.0 → equal");
  }

  /* Should be FALSE — at least one zero → NaN */
  printf("\n  --- zero cases (expect FAIL = return 0) ---\n");

  /*
   * Manual's counter-example: x=0, z=1
   *   dx/dx = 0.0/0.0 = NaN
   *   dz/dz = 1.0/1.0 = 1.0
   *   NaN == 1.0 → false
   */
  {
    double dx = 0.0, dz = 1.0;
    volatile double lhs = dx / dx;
    volatile double rhs = dz / dz;
    printf("\n  Diagnostic for x=0, z=1:\n");
    print_bits("dx/dx (0/0)", lhs);
    print_bits("dz/dz (1/1)", rhs);
    printf("         isnan(dx/dx) = %d\n", isnan(lhs));
    expect(condE(dx, dz), 0, "condE(dx=0, dz=1)",
        "manual counter-example: 0/0=NaN, 1/1=1.0, NaN!=1.0");
  }
  /* Both zero: NaN != NaN */
  {
    double dx = 0.0, dz = 0.0;
    expect(condE(dx, dz), 0, "condE(dx=0, dz=0)",
        "NaN != NaN — even the same NaN is not equal to itself");
  }
  /* z=0, x nonzero */
  {
    double dx = (double)INT_MAX, dz = 0.0;
    expect(condE(dx, dz), 0, "condE(dx=INT_MAX, dz=0)",
        "dz/dz=NaN != 1.0 = dx/dx");
  }
  /* x=0, z=INT_MIN (nonzero) */
  {
    double dx = 0.0, dz = (double)INT_MIN;
    expect(condE(dx, dz), 0, "condE(dx=0, dz=INT_MIN)",
        "dx/dx=NaN != 1.0 = dz/dz");
  }
}

/* ── Main ────────────────────────────────────────────────────────────────── */
int main(void) {
  printf(BOLD
      "═══════════════════════════════════════════════════════\n"
      "  CSAPP 3e — Problem 2.89 boundary-condition test suite\n"
      "═══════════════════════════════════════════════════════\n"
      RST);

  printf(DIM
      "  Compile flags needed for D-failures:\n"
      "    -mfpmath=sse -msse2 -fno-associative-math -mno-fma\n"
      RST);

  test_A();
  test_B();
  test_C();
  test_D();
  test_E();

  int total = g_pass + g_fail;
  printf(BOLD
      "\n═══════════════════════════════════════════════════════\n"
      "  Results: %d/%d passed", g_pass, total);
  if (g_fail == 0)
    printf("  " GREEN "ALL PASS" RST);
  else
    printf("  " RED "%d FAILED" RST, g_fail);
  printf(BOLD "\n"
      "═══════════════════════════════════════════════════════\n"
      RST);

  return g_fail ? 1 : 0;
}
