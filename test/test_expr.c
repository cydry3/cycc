int expect(int actual, int expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}

// Postfix operators
// Function calls
int Function_call_op1() { foo(); }
int Function_call_op2() { bar(31); }
int Function_call_op3() { buzz(3, 4); }
int Function_call_op4() { fizz(1, 2, 3, 4, 5, 6); }

// Unary operators
// Address, indiretions operators
int Address_indiretions_op1() {
  int *p;
  int *q;
  alloc4(&p, 1, 2, 4, 8);
  q = p;
  return (*q);
}
int Address_indiretions_op2() {
  int *p;
  int *q;
  alloc4(&p, 1, 2, 4, 8);
  q = p + 2;
  return (*q);
}
int Address_indiretions_op3() {
  int *p;
  int *q;
  alloc4(&p, 1, 2, 4, 8);
  q = p + 3;
  q = q - 2;
  return (*q);
}

// sizeof operators
int Sizeof_op1() {
  int x;
  return sizeof(x);
}
int Sizeof_op2() {
  int *y_ptr;
  return sizeof(y_ptr);
}
int Sizeof_op3() {
  int x;
  return sizeof(x + 3);
}
int Sizeof_op4() {
  int *y_ptr;
  return sizeof(y_ptr + 3);
}
int Sizeof_op5() {
  int *y_ptr;
  return sizeof(*y_ptr);
}
int Sizeof_op6() { return sizeof(1); }
int Sizeof_op7() { return sizeof(sizeof(1)); }
int Sizeof_op8() { return sizeof(1 == 1); }
int Sizeof_op9() { return sizeof(1 != 1); }
int Sizeof_op10() { return sizeof(1 < 1); }
int Sizeof_op11() { return sizeof(1 <= 1); }
int Sizeof_op12() { return sizeof(1 > 1); }
int Sizeof_op13() { return sizeof(1 >= 1); }
int Sizeof_op14() { return sizeof(1 * 2); }
int Sizeof_op15() { return sizeof(4 / 2); }
int Sizeof_op16() { return sizeof(1 + 2); }
int Sizeof_op17() { return sizeof(3 - 1); }
int Sizeof_op18() {
  int x;
  return sizeof(x - 2);
}
int Sizeof_op19() {
  int y_ptr;
  return sizeof(y_ptr - 3);
}
int Sizeof_op20() {
  int y_ptr;
  return sizeof(&y_ptr);
}
int Sizeof_op21() {
  int x;
  return sizeof(x + 3 + 4);
}
int Sizeof_op22() { return sizeof(3 * (1 + 2)); }
int Sizeof_op23() { return sizeof((3 + 1) > 1); }
int Sizeof_op24() {
  int *x_ptr;
  int y;
  x_ptr = &y;
  return sizeof((*x_ptr + 3) > 4);
}
int Sizeof_op25() {
  int *x_ptr;
  x_ptr = &y;
  return sizeof((x_ptr + 3) + 4);
}
int Sizeof_op26() {
  int a;
  return sizeof(a = 1);
}
int Sizeof_op27() {
  int a;
  int *s_ptr;
  s_ptr = &a;
  return sizeof(*s_ptr = 1);
}
int Sizeof_op28() {
  int *s_ptr;
  return sizeof(s_ptr = 1);
}

// Assignment operators
int Assignment_op1() {
  int a;
  a = 5;
}
int Assignment_op2() {
  int a;
  int b;
  a = b = 2;
}
int Assignment_op3() {
  int a;
  int b;
  int c;
  a = b = 3;
  c = 2;
}
int Assignment_op4() {
  int a;
  int b;
  int c;
  a = b = 3;
  c = 2;
}
int Assignment_op5() {
  int a;
  int b;
  a = 5;
  b = 3;
  a;
}
int Assignment_op6() {
  int a;
  int b;
  int c;
  a = b = 3;
  a = 5;
  c = 2;
  a;
}

int main() {
  int got;
  int want;
  char *msg;

  // Postfix operators
  // Function calls
  msg = "Function call op.1";
  got = Function_call_op1();
  want = 1;
  expect(got, want, msg);

  msg = "Function call op.2";
  got = Function_call_op2();
  want = 31;
  expect(got, want, msg);

  msg = "Function call op.3";
  got = Function_call_op3();
  want = 7;
  expect(got, want, msg);

  msg = "Function call op.4";
  got = Function_call_op4();
  want = 21;
  expect(got, want, msg);

  // Additive operators
  msg = "Additive op.1";
  got = (5 + 20 - 4);
  want = 21;
  expect(got, want, msg);

  msg = "Additive op.2";
  got = (12 + 34 - 5);
  want = 41;
  expect(got, want, msg);

  msg = "Additive op.3";
  got = (5 + 20 - 4);
  want = 21;
  expect(got, want, msg);

  msg = "Additive op.4";
  got = (12 + 34 - 5);
  want = 41;
  expect(got, want, msg);

  msg = "Additive op.5";
  got = ((3 + 5) / 2);
  want = 4;
  expect(got, want, msg);

  msg = "Additive op.6";
  got = (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1);
  want = 50;
  expect(got, want, msg);

  msg = "Additive op.7";
  got = (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1);
  want = 51;
  expect(got, want, msg);

  msg = "Additive op.8";
  got = (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 +
         1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1);
  want = 100;
  expect(got, want, msg);

  // Unary operators
  msg = "Unary op.1";
  got = (-25 + 45);
  want = 20;
  expect(got, want, msg);

  msg = "Unary op.2";
  got = (93 + (-25));
  want = 68;
  expect(got, want, msg);
  expect(got, want, msg);

  // Address, indiretions operators
  msg = "Address indiretions op.1";
  got = Address_indiretions_op1();
  want = 1;
  expect(got, want, msg);

  msg = "Address indiretions op.2";
  got = Address_indiretions_op2();
  want = 4;
  expect(got, want, msg);

  msg = "Address indiretions op.3";
  got = Address_indiretions_op3();
  want = 2;
  expect(got, want, msg);

  // Sizeof operators
  msg = "Sizeof op.1";
  got = Sizeof_op1();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.2";
  got = Sizeof_op2();
  want = 8;
  expect(got, want, msg);

  msg = "Sizeof op.3";
  got = Sizeof_op3();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.4";
  got = Sizeof_op4();
  want = 8;
  expect(got, want, msg);

  msg = "Sizeof op.5";
  got = Sizeof_op5();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.6";
  got = Sizeof_op6();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.7";
  got = Sizeof_op7();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.8";
  got = Sizeof_op8();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.9";
  got = Sizeof_op9();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.10";
  got = Sizeof_op10();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.11";
  got = Sizeof_op11();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.12";
  got = Sizeof_op12();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.13";
  got = Sizeof_op13();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.14";
  got = Sizeof_op14();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.15";
  got = Sizeof_op15();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.16";
  got = Sizeof_op16();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.17";
  got = Sizeof_op17();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.18";
  got = Sizeof_op18();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.19";
  got = Sizeof_op19();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.20";
  got = Sizeof_op20();
  want = 8;
  expect(got, want, msg);

  msg = "Sizeof op.21";
  got = Sizeof_op21();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.22";
  got = Sizeof_op22();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.23";
  got = Sizeof_op23();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.24";
  got = Sizeof_op24();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.25";
  got = Sizeof_op25();
  want = 8;
  expect(got, want, msg);

  msg = "Sizeof op.26";
  got = Sizeof_op26();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.27";
  got = Sizeof_op27();
  want = 4;
  expect(got, want, msg);

  msg = "Sizeof op.28";
  got = Sizeof_op28();
  want = 8;
  expect(got, want, msg);

  // Multilpicative operators
  msg = "Multilpicative op.1";
  got = (5 + 6 * 7);
  want = 47;
  expect(got, want, msg);

  msg = "Multilpicative op.2";
  got = (5 * (9 - 6));
  want = 15;
  expect(got, want, msg);

  msg = "Equality op.1";
  got = (1 == 1);
  want = 1;
  expect(got, want, msg);

  msg = "Equality op.2";
  got = (1 == 0);
  want = 0;
  expect(got, want, msg);

  msg = "Equality op.3";
  got = (1 != 0);
  want = 1;
  expect(got, want, msg);

  msg = "Equality op.4";
  got = (1 != 1);
  want = 0;
  expect(got, want, msg);

  // Relational operators
  msg = "Relational op.1";
  got = (0 < 1);
  want = 1;
  expect(got, want, msg);

  msg = "Relational op.2";
  got = (0 > 1);
  want = 0;
  expect(got, want, msg);

  msg = "Relational op.3";
  got = (1 > 0);
  want = 1;
  expect(got, want, msg);

  msg = "Relational op.4";
  got = (1 < 0);
  want = 0;
  expect(got, want, msg);

  msg = "Relational op.5";
  got = (1 < 1);
  want = 0;
  expect(got, want, msg);

  msg = "Relational op.6";
  got = (1 > 1);
  want = 0;
  expect(got, want, msg);

  msg = "Relational op.7";
  got = (0 <= 1);
  want = 1;
  expect(got, want, msg);

  msg = "Relational op.8";
  got = (0 >= 1);
  want = 0;
  expect(got, want, msg);

  msg = "Relational op.9";
  got = (1 >= 0);
  want = 1;
  expect(got, want, msg);

  msg = "Relational op.10";
  got = (1 <= 0);
  want = 0;
  expect(got, want, msg);

  msg = "Relational op.11";
  got = (1 <= 1);
  want = 1;
  expect(got, want, msg);

  msg = "Relational op.12";
  got = (1 >= 1);
  want = 1;
  expect(got, want, msg);

  // Assignment operators
  msg = "Assignment op.1";
  got = Assignment_op1();
  want = 5;
  expect(got, want, msg);

  msg = "Assignment op.2";
  got = Assignment_op2();
  want = 2;
  expect(got, want, msg);

  msg = "Assignment op.3";
  got = Assignment_op3();
  want = 2;
  expect(got, want, msg);

  msg = "Assignment op.4";
  got = Assignment_op4();
  want = 2;
  expect(got, want, msg);

  msg = "Assignment op.5";
  got = Assignment_op5();
  want = 5;
  expect(got, want, msg);

  msg = "Assignment op.6";
  got = Assignment_op5();
  want = 5;
  expect(got, want, msg);

  return 0;
}
