int queen[8];

int check_by_once(int *target) {
  int i;
  int j;
  int a;
  int b;
  int m;
  int n;

  for (i = 0; i < 7; i = i + 1) {
    for (j = i + 1; j < 8; j = j + 1) {
      a = target[i];
      b = target[j];
      if (a == b)
        return 0;
      m = a - b;
      n = j - i;
      if ((m) == (n))
        return 0;
    }
  }
  return 1;
}

int main() {
  int i;
  int res;

  // test check_by_once function

  res = check_by_once(queen);
  print_i(res);
  print_ln();

  for (i = 0; i < 8; i = i + 1) {
    queen[i] = i;
  }

  // print values of array 'queen'
  for (i = 0; i < 8; i = i + 1) {
    print_i(queen[i]);
  }
  print_ln();

  // test check_by_once function
  res = check_by_once(queen);
  print_i(res);
  print_ln();

  return 0;
}
