int expect(int actual, int expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}

// Declarators
// Array declarators
int Array_decl1() {
  int ary[10];
  return 1;
}
int Array_decl2() {
  int ary[2];
  *ary = 3;
  int *p;
  p = ary;
  return (*p);
}
int Array_decl3() {
  int a[2];
  *a = 1;
  *(a + 1) = 2;
  int *p;
  p = a;
  return *p + *(p + 1);
}
int Array_decl4() {
  int a[10];
  *(a + 2) = 3;
  int *p;
  p = a + 2;
  return *p;
}
int Array_decl5() {
  int a[10];
  *(a + 5) = 7;
  return (*(a + 5));
}
int Array_decl6() {
  int a[21];
  *a = 1;
  *(a + 1) = 2;
  int *p;
  p = a;
  return *p;
}
int Array_decl7() {
  int a[10];
  *a = 1;
  *(a + 1) = 2;
  int *p;
  p = a;
  return *p + *(p + 1);
}
int Array_decl8() {
  int a[100];
  *a = 2;
  *(a + 11) = 3;
  int *p;
  p = a;
  return *p + *(p + 11);
}
int Array_decl9() {
  int a[2];
  a[0] = 1;
  return a[0];
}
int Array_decl10() {
  int a[100];
  a[0] = 2;
  a[11] = 3;
  return a[0] + a[11];
}
int Array_decl11() {
  int a[10];
  int i;
  for (i = 0; i < 10; i = i + 1) {
    a[i] = i;
  }
  return a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8] + a[9];
}
int Array_decl12() {
  char c_ary[10];
  c_ary[0] = 1;
  c_ary[1] = 2;
  return c_ary[0] + c_ary[1];
}
int Array_decl13() {
  char c_ary[3];
  int y;
  c_ary[0] = -1;
  c_ary[1] = 2;
  y = 4;
  return c_ary[0] + y;
}

int main() {
  int got;
  int want;
  char *msg;

  // Declarators
  // Array declarators
  msg = "Array decl.1";
  got = Array_decl1();
  want = 1;
  expect(got, want, msg);

  msg = "Array decl.2";
  got = Array_decl2();
  want = 3;
  expect(got, want, msg);

  msg = "Array decl.3";
  got = Array_decl3();
  want = 3;
  expect(got, want, msg);

  msg = "Array decl.4";
  got = Array_decl4();
  want = 3;
  expect(got, want, msg);

  msg = "Array decl.5";
  got = Array_decl5();
  want = 7;
  expect(got, want, msg);

  msg = "Array decl.6";
  got = Array_decl6();
  want = 1;
  expect(got, want, msg);

  msg = "Array decl.7";
  got = Array_decl7();
  want = 3;
  expect(got, want, msg);

  msg = "Array decl.8";
  got = Array_decl8();
  want = 5;
  expect(got, want, msg);

  msg = "Array decl.9";
  got = Array_decl9();
  want = 1;
  expect(got, want, msg);

  msg = "Array decl.10";
  got = Array_decl10();
  want = 5;
  expect(got, want, msg);

  msg = "Array decl.11";
  got = Array_decl11();
  want = 45;
  expect(got, want, msg);

  /* TODO
  msg = "Array decl.12";
  got = Array_decl12();
  printbar("debug:%d\n", got);
  want = 1;
  expect(got, want, msg);

  msg = "Array decl.13";
  got = Array_decl13();
  want = 3;
  expect(got, want, msg);
  */

  return 0;
}
