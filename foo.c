#include <stdio.h>

int foo() { printf("OK\n"); }
int bar(int x) { printf("%d\n", x); }
int buzz(int x, int y) { printf("%d\n", x + y); }
int fizz(int a, int b, int c, int d, int e, int f) {
  printf("%d\n", a + b + c + d + e + f);
}
