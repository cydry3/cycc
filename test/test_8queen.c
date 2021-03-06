int queen[8];
int count;

int abs(int x) { return absfoo(x); }

int check_line(int *target) {
  int i;
  int j;
  int a;
  int b;

  for (i = 0; i < 7; i = i + 1) {
    for (j = i + 1; j < 8; j = j + 1) {
      a = target[i];
      b = target[j];
      if (a == b)
        return 0;
    }
  }
  return 1;
}

int check_diagonal(int *target) {
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
      m = a - b;
      n = j - i;
      if (abs(m) == abs(n))
        return 0;
    }
  }
  return 1;
}

int check(int *target) {
  if (check_line(target) == 0)
    return 0;

  if (check_diagonal(target) == 0)
    return 0;

  return 1;
}

int print_queen(int *q) {
  int i;
  int val;

  for (i = 0; i < 8; i = i + 1) {
    val = *(q + i);
    printbar("%d ", val);
  }
  printfoo("\n");
}

int set_queen(int n) {
  int i;

  if (n == 8) {
    if (check(queen)) {
      count = count + 1;
      print_queen(queen);
    }
    return 0;
  }

  for (i = 0; i < 8; i = i + 1) {
    *(queen + n) = i;
    set_queen((n + 1));
  }
  return 0;
}

int main() {
  count = 0;

  set_queen(0);

  return count;
}
