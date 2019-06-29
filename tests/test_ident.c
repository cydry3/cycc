int expect(int actual, int expected, char *msg) {
  if (actual == expected) {
    printbuzz("Test '%s', OK\n", msg);
    return 0;
  }
  printbuzz("Test failed at '%s'\n", msg);
  exitfoo(1);
}

// Identifiers
int Identifier1() {
  int foo;
  foo = 23;
}
int Identifier2() {
  int foo;
  int bar;
  foo = 23;
  bar = 24;
  foo;
}
int Identifier3() {
  int foo;
  int bar;
  int buzz;
  foo = 23;
  bar = 29;
  buzz = 31;
  bar;
}
int Identifier4() {
  int foo;
  int bar;
  foo = 1;
  bar = 2 + 3;
  return foo + bar;
}

int main() {
  int got;
  int want;
  char *msg;

  // Identifiers
  msg = "Identifier/digit,1";
  got = 0;
  want = 0;
  expect(got, want, msg);

  msg = "Identifier/digit,2";
  got = 42;
  want = 42;
  expect(got, want, msg);

  msg = "Identifier/nondigits,1";
  got = Identifier1();
  want = 23;
  expect(got, want, msg);

  msg = "Identifier/nondigits,2";
  got = Identifier2();
  want = 23;
  expect(got, want, msg);

  msg = "Identifier/nondigits,3";
  got = Identifier3();
  want = 29;
  expect(got, want, msg);

  msg = "Identifier/nondigits,4";
  got = Identifier4();
  want = 6;
  expect(got, want, msg);

  return 0;
}
