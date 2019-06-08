#include <stdio.h>
#include <stdlib.h>

int foo() { printf("OK\n"); }
int bar(int x) { printf("%d\n", x); }
int buzz(int x, int y) { printf("%d\n", x + y); }
int fizz(int a, int b, int c, int d, int e, int f) {
  printf("%d\n", a + b + c + d + e + f);
}
int is_equal_ptrs(void *p1, void *p2) { printf("%d", (p1 == p2)); }
int alloc4(int **y, int a, int b, int c, int d) {
  int *x;

  x = (int *)malloc(sizeof(int) * 4);
  x[0] = a;
  x[1] = b;
  x[2] = c;
  x[3] = d;

  *y = x;
}
int printfoo(char *str) { printf(str); }
