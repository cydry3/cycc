int expect(int actual, int expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}
// Declarators
// Pointer declarators
int Pointer_decl1() {
  int *ptr;
  int b;
  b = 5;
  ptr = &b;
  *ptr = 7;
  return b;
}
int Pointer_decl2() {
  int *ptr;
  int b;
  b = 5;
  ptr = &b;
  *ptr = 0;
  return b;
}
int Pointer_decl3() {
  int *ptr;
  int b;
  int c;
  b = 5;
  ptr = &b;
  c = *ptr;
  return c;
}
int Pointer_decl4() {
  int *ptr;
  int b;
  int c;
  b = 5;
  ptr = &b;
  *ptr = 0;
  c = *ptr;
  return c;
}
int Pointer_decl5() {
  int x;
  int *ptr;
  x = 3;
  ptr = &x;
  return (*ptr);
}
int Pointer_decl6() {
  int x;
  int *ptr;
  x = 3;
  ptr = &x;
  return *ptr;
}
int Pointer_decl7() {
  int **x_ptr;
  int *y_ptr;
  int z;
  x_ptr = &y_ptr;
  y_ptr = &z;
  **x_ptr = 3;
  return z;
}
int Pointer_decl8() {
  int **x_ptr;
  int *y_ptr;
  int z;
  x_ptr = &y_ptr;
  y_ptr = &z;
  z = 5;
  return **x_ptr;
}
int Pointer_decl9() {
  int a;
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  ***x_ptr = 7;
  return a;
}
int Pointer_decl10() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  a = 11;
  return ***x_ptr;
}
int Pointer_decl11() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  ***x_ptr = 11;
  **y_ptr = 13;
  return a;
}
int Pointer_decl12() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  return is_equal_ptrs(**x_ptr, z_ptr);
}
int Pointer_decl13() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  return is_equal_ptrs(*y_ptr, z_ptr);
}
int Pointer_decl14() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  return is_equal_ptrs(*y_ptr, z_ptr);
}
int Pointer_decl15() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  return is_equal_ptrs(*x_ptr, y_ptr);
}
int Pointer_decl16() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  return is_equal_ptrs(***x_ptr, &a);
}
int Pointer_decl17() {
  int ***x_ptr;
  int **y_ptr;
  int *z_ptr;
  int a;
  x_ptr = &y_ptr;
  y_ptr = &z_ptr;
  z_ptr = &a;
  return is_equal_ptrs(***x_ptr, a);
}
int Pointer_ext18(int *a_ptr) { return (*a_ptr); }
int Pointer_decl18() {
  int x;
  int *y_ptr;
  x = 3;
  y_ptr = &x;
  return Pointer_ext18(y_ptr);
}
int Pointer_ext19(int *x_ptr, int *y_ptr) {
  int ret;
  ret = *x_ptr + *y_ptr;
  return ret;
}
int Pointer_decl19() {
  int *a_ptr;
  int *b_ptr;
  int c;
  int d;
  a_ptr = &c;
  b_ptr = &d;
  *a_ptr = 3;
  *b_ptr = 4;
  return Pointer_ext19(a_ptr, b_ptr);
}

int main() {
  int got;
  int want;
  char *msg;

  // Declarators
  // Pointer declarators
  msg = "Pointer decl.1";
  got = Pointer_decl1();
  want = 7;
  expect(got, want, msg);

  msg = "Pointer decl.2";
  got = Pointer_decl2();
  want = 0;
  expect(got, want, msg);

  msg = "Pointer decl.3";
  got = Pointer_decl3();
  want = 5;
  expect(got, want, msg);

  msg = "Pointer decl.4";
  got = Pointer_decl4();
  want = 0;
  expect(got, want, msg);

  msg = "Pointer decl.5";
  got = Pointer_decl5();
  want = 3;
  expect(got, want, msg);

  msg = "Pointer decl.6";
  got = Pointer_decl6();
  want = 3;
  expect(got, want, msg);

  msg = "Pointer decl.7";
  got = Pointer_decl7();
  want = 3;
  expect(got, want, msg);

  msg = "Pointer decl.8";
  got = Pointer_decl8();
  want = 5;
  expect(got, want, msg);

  msg = "Pointer decl.9";
  got = Pointer_decl9();
  want = 7;
  expect(got, want, msg);

  msg = "Pointer decl.10";
  got = Pointer_decl10();
  want = 11;
  expect(got, want, msg);

  msg = "Pointer decl.11";
  got = Pointer_decl11();
  want = 13;
  expect(got, want, msg);

  msg = "Pointer decl.12";
  got = Pointer_decl12();
  want = 1;
  expect(got, want, msg);

  msg = "Pointer decl.13";
  got = Pointer_decl13();
  want = 1;
  expect(got, want, msg);

  msg = "Pointer decl.14";
  got = Pointer_decl14();
  want = 1;
  expect(got, want, msg);

  msg = "Pointer decl.15";
  got = Pointer_decl15();
  want = 1;
  expect(got, want, msg);

  msg = "Pointer decl.16";
  got = Pointer_decl16();
  want = 0;
  expect(got, want, msg);

  msg = "Pointer decl.17";
  got = Pointer_decl17();
  want = 1;
  expect(got, want, msg);

  msg = "Pointer decl.18";
  got = Pointer_decl18();
  want = 3;
  expect(got, want, msg);

  msg = "Pointer decl.19";
  got = Pointer_decl19();
  want = 7;
  expect(got, want, msg);

  return 0;
}
