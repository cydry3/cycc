#include <stdio.h>
#include <stdlib.h>

int print_ok() { printf("OK"); }
int print_i(int x) { printf("%d", x); }
int print_add_i(int x, int y) { printf("%d", (x + y)); }
int print_sum_i(int a, int b, int c, int d, int e, int f) {
  printf("%d", (a + b + c + d + e + f));
}

int foo() { return 1; }
int bar(int x) { return x; }
int buzz(int x, int y) { return (x + y); }
int fizz(int a, int b, int c, int d, int e, int f) {
  return (a + b + c + d + e + f);
}
int is_equal_ptrs(void *p1, void *p2) { return (p1 == p2); }
int alloc4(int **y, int a, int b, int c, int d) {
  int *x;

  x = (int *)malloc(sizeof(int) * 4);
  x[0] = a;
  x[1] = b;
  x[2] = c;
  x[3] = d;

  *y = x;
  return 0;
}
int printfoo(char *str) { printf(str); }
int printbar(char *str, int n) { printf(str, n); }
int printbuzz(char *str, char *msg) { printf(str, msg); }
int absfoo(int n) { return abs(n); }
int exitfoo(int s) { exit(s); }
