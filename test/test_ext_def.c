int expect(int actual, int expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}

char expect_c(char actual, char expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}

// External definitions
// Function definitions
int func_def_ext1() { foo(); }
int Function_def1() { func_def_ext1(); }
int func_def_ext2() { return 9; }
int Function_def2() {
  int a;
  a = func_def_ext2();
  bar(a);
}
int func_def_ext3(int x) {
  int a;
  a = x;
  bar(a);
}
int Function_def3() { func_def_ext3(9); }
int func_def_ext4(int x, int y) {
  int a;
  a = x + y;
  bar(a);
}
int Function_def4() { func_def_ext4(11, 31); }
int func_def_ext5(int a, int b, int c, int d, int e, int f) {
  int x;
  int y;
  x = a + b * c;
  y = d * e - f;
  buzz(x, y);
}
int Function_def5() { func_def_ext5(1, 2, 3, 4, 5, 6); }
int func_def_ext6(int x, int y) { return x + y; }
int Function_def6() {
  int a;
  a = func_def_ext6(3, 4);
  bar(a);
}
int fib(int n) {
  if (n == 0)
    return 0;
  if (n == 1)
    return 1;
  return fib(n - 2) + fib(n - 1);
}
int Function_def7() {
  int a;
  a = fib(5);
  return a;
}
int Function_def8() {
  int a;
  a = fib(21);
  return a;
}
int Obj_a1;
int Object_def1() { return Obj_a1; }

int Obj_a2;
int Object_def2() {
  Obj_a2 = 1;
  return Obj_a2;
}

int Object_def3() {
  Obj_a1 = 1;
  Obj_a2 = 2;
  return Obj_a1 + Obj_a2;
}

int Obj_ary[10];
int Object_def4() { return Obj_ary[0]; }
int Object_def5() {
  Obj_ary[0] = 2;
  return Obj_ary[0];
}

int Obj_ary100[100];
int Object_def6() {
  Obj_ary100[0] = 2;
  Obj_ary100[11] = 3;
  return Obj_ary100[0] + Obj_ary100[11];
}
int Object_def7() {
  int x;
  x = 11;
  Obj_ary100[0] = 2;
  Obj_ary100[x] = 3;
  return Obj_ary100[0] + Obj_ary100[x];
}

int Obj_i;
int Object_def8() {
  for (Obj_i = 0; Obj_i < 10; Obj_i = Obj_i + 1) {
    Obj_ary[Obj_i] = Obj_i;
  }
  return Obj_ary[0] + Obj_ary[1] + Obj_ary[2] + Obj_ary[3] + Obj_ary[4] +
         Obj_ary[5] + Obj_ary[6] + Obj_ary[7] + Obj_ary[8] + Obj_ary[9];
}

char Obj_c;
int Object_def9() {
  Obj_c = 1;
  return Obj_c;
}

char Obj_ary_c[10];
int Object_def10() {
  Obj_ary_c[0] = -1;
  Obj_ary_c[1] = 2;
  return Obj_ary_c[0] + Obj_ary_c[1];
}
char Obj_ary_c3[3];
int Object_def11() {
  int y;
  Obj_ary_c3[0] = -1;
  Obj_ary_c3[1] = 2;
  y = 4;
  return Obj_ary_c3[0] + y;
}

int main() {
  int got;
  int want;
  char *msg;

  // External definitions
  // Function definitions
  msg = "Function_def.1";
  got = Function_def1();
  want = 1;
  expect(got, want, msg);

  msg = "Function_def.2";
  got = Function_def2();
  want = 9;
  expect(got, want, msg);

  msg = "Function_def.3";
  got = Function_def3();
  want = 9;
  expect(got, want, msg);

  msg = "Function_def.4";
  got = Function_def4();
  want = 42;
  expect(got, want, msg);

  msg = "Function_def.5";
  got = Function_def5();
  want = 21;
  expect(got, want, msg);

  msg = "Function_def.6";
  got = Function_def6();
  want = 7;
  expect(got, want, msg);

  msg = "Function_def.7";
  got = Function_def7();
  want = 5;
  expect(got, want, msg);

  msg = "Function_def.8";
  got = Function_def8();
  want = 10946;
  expect(got, want, msg);

  // Object definitions
  msg = "Object def.1";
  got = Object_def1();
  want = 0;
  expect(got, want, msg);

  msg = "Object def.2";
  got = Object_def2();
  want = 1;
  expect(got, want, msg);

  msg = "Object def.3";
  got = Object_def3();
  want = 3;
  expect(got, want, msg);

  msg = "Object def.4";
  got = Object_def4();
  want = 0;
  expect(got, want, msg);

  msg = "Object def.5";
  got = Object_def5();
  want = 2;
  expect(got, want, msg);

  msg = "Object def.6";
  got = Object_def6();
  want = 5;
  expect(got, want, msg);

  msg = "Object def.7";
  got = Object_def7();
  want = 5;
  expect(got, want, msg);

  msg = "Object def.8";
  got = Object_def8();
  want = 45;
  expect(got, want, msg);

  msg = "Object def.9";
  got = Object_def9();
  want = 1;
  expect(got, want, msg);

  char c_got;
  char c_want;
  msg = "Object def.10";
  c_got = Object_def10();
  c_want = 1;
  expect_c(c_got, c_want, msg);

  msg = "Object def.11";
  c_got = Object_def11();
  c_want = 3;
  expect_c(c_got, c_want, msg);

  return 0;
}
