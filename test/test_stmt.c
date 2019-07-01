int expect(int actual, int expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}
// Statements
// Selection Statements
int If_stmt1() {
  if (1)
    2;
}
int If_stmt2() {
  int a;
  a = 1;
  if (a == 1)
    a = 7;
  return a;
}
int If_stmt3() {
  int a;
  a = 1;
  if (a != 1)
    a = 7;
  return a;
}
int If_stmt4() {
  int a;
  a = 1;
  if (a == 1)
    a = 7;
  else
    a = 3;
  return a;
}
int If_stmt5() {
  int a;
  a = 1;
  if (a != 1)
    a = 7;
  else
    a = 3;
  return a + 2;
}
int If_stmt6() {
  int a;
  a = 1;
  if (a > 0) {
    if (a != 1)
      a = 7;
    else
      a = 3;
  }
  return a + 2;
}
int If_stmt7() {
  if (1) {
    return 2;
  }
}
int If_stmt8() {
  int a;
  a = 1;
  if (a) {
    a = 7;
    a = 8;
  }
  return a;
}

// Iteration Statements
int While_stmt1() {
  int i;
  i = 0;
  while (i < 10)
    i = i + 1;
  return i;
}
int While_stmt2() {
  int i;
  int a;
  i = 0;
  a = 0;
  while (i < 10) {
    i = i + 1;
    a = i;
  }
  return a;
}
int For_stmt1() {
  int a;
  int i;
  a = 0;
  for (i = 0; i < 5; i = i + 1)
    a = a + i;
  return a;
}
int For_stmt2() {
  int a;
  int i;
  a = 1;
  for (i = 5; i > 0; i = i - 1)
    a = a * i;
  return a;
}
int For_stmt3() {
  int a;
  int val;
  int i;
  a = 1;
  val = 0;
  for (i = 5; i > 0; i = i - 1) {
    a = a * i;
    val = a;
  }
  return val;
}

// Jump Statements
int Return_stmt1() { return 11; }
int Return_stmt2() {
  int a;
  int b;
  a = 3;
  b = 5 * 6 - 8;
  return a + b / 2;
}
int Return_stmt3() {
  return 5;
  return 8;
}

int main() {
  int got;
  int want;
  char *msg;

  // Selection Statements
  msg = "if stmt.1";
  got = If_stmt1();
  want = 2;
  expect(got, want, msg);

  msg = "if stmt.2";
  got = If_stmt2();
  want = 7;
  expect(got, want, msg);

  msg = "if stmt.3";
  got = If_stmt3();
  want = 1;
  expect(got, want, msg);

  msg = "if stmt.4";
  got = If_stmt4();
  want = 7;
  expect(got, want, msg);

  msg = "if stmt.5";
  got = If_stmt5();
  want = 5;
  expect(got, want, msg);

  msg = "if stmt.6";
  got = If_stmt6();
  want = 5;
  expect(got, want, msg);

  msg = "if stmt.7";
  got = If_stmt7();
  want = 2;
  expect(got, want, msg);

  msg = "if stmt.8";
  got = If_stmt8();
  // Selection Statements
  msg = "if stmt.1";
  got = If_stmt1();
  want = 2;
  expect(got, want, msg);

  msg = "if stmt.2";
  got = If_stmt2();
  want = 7;
  expect(got, want, msg);

  msg = "if stmt.3";
  got = If_stmt3();
  want = 1;
  expect(got, want, msg);

  msg = "if stmt.4";
  got = If_stmt4();
  want = 7;
  expect(got, want, msg);

  msg = "if stmt.5";
  got = If_stmt5();
  want = 5;
  expect(got, want, msg);

  msg = "if stmt.6";
  got = If_stmt6();
  want = 5;
  expect(got, want, msg);

  msg = "if stmt.7";
  got = If_stmt7();
  want = 2;
  expect(got, want, msg);

  msg = "if stmt.8";
  got = If_stmt8();
  want = 8;
  expect(got, want, msg);

  // Iteration Statements
  msg = "While stmt.1";
  got = While_stmt1();
  want = 10;
  expect(got, want, msg);

  msg = "While stmt.2";
  got = While_stmt2();
  want = 10;
  expect(got, want, msg);

  msg = "For stmt.1";
  got = For_stmt1();
  want = 10;
  expect(got, want, msg);

  msg = "For stmt.2";
  got = For_stmt2();
  want = 120;
  expect(got, want, msg);

  msg = "For stmt.3";
  got = For_stmt3();
  want = 120;
  expect(got, want, msg);

  // Jump Statements
  msg = "Return stmt.1";
  got = Return_stmt1();
  want = 11;
  expect(got, want, msg);

  msg = "Return stmt.2";
  got = Return_stmt2();
  want = 14;
  expect(got, want, msg);

  msg = "Return stmt.3";
  got = Return_stmt3();
  want = 5;
  expect(got, want, msg);

  return 0;
}
