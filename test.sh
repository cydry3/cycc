#!/bin/bash
try() {
    if [ "$1" = "-out" ]; then
	expected="$2"
	input="$3"
	./cycc "$input" > tmp.s
    elif [ "$2" = "-file" ]; then
	expected="$1"
	input="$3"
	./cycc "$2" "$input" > tmp.s
    else
	expected="$1"
	input="$2"
	./cycc "$input" > tmp.s
    fi


    if [ "$1" = "-out" ]; then
	gcc -o tmp tmp.s foo.o
	actual=`./tmp`
    else
	gcc -o tmp tmp.s foo.o
	./tmp
	actual="$?"
    fi


    if [ "$actual" = "$expected" ]; then
	echo "$input => $actual"
    else
	echo "$input => $actual"
	echo "$expected expected, but got $actual"
	exit 1
    fi
}

# Test C files
try 0 -file "test/test_ident.c"
try 0 -file "test/test_ptr.c"
try 0 -file "test/test_array.c"
try 0 -file "test/test_expr.c"
try 0 -file "test/test_stmt.c"
try 0 -file "test/test_ext_def.c"
try 0 -file "test/test_comment.c"

Test using stdout
try 0 "int main(){ 0; }"
try 42 "int main(){ 42;}"
try 21 "int main(){ 5+20-4;}"
try 41 "int main(){  12 + 34 - 5 ;}"
try 47 "int main(){ 5+6*7;}"
try 15 "int main(){ 5*(9-6);}"
try 4 "int main(){ (3+5)/2;}"
try 20 "int main(){ -25+45;}"
try 68 "int main(){ 93+(-25);}"
try 1 "int main(){ 1==1;}"
try 0 "int main(){ 1==0;}"
try 1 "int main(){ 1!=0;}"
try 0 "int main(){ 1!=1;}"
try 0 "int main(){ 1<=0;}"
try 1 "int main(){ 1<=1;}"
try 1 "int main(){ 0<=1;}"
try 1 "int main(){ 1>=0;}"
try 1 "int main(){ 1>=1;}"
try 0 "int main(){ 0>=1;}"
try 0 "int main(){ 0>=1;}"
try 0 "int main(){ 1<1;}"
try 1 "int main(){ 0<1;}"
try 0 "int main(){ 1<0;}"
try 0 "int main(){ 1>1;}"
try 0 "int main(){ 0>1;}"
try 1 "int main(){ 1>0;}"
try 50 "int main(){ 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;}"
try 51 "int main(){ 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;}"
try 100 "int main(){ 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;}"
try 5 "int main(){ int a; a = 5;}"
try 2 "int main(){ int a; int b; a = b = 2;}"
try 2 "int main(){ int a; int b; int c; a = b = 3; c = 2;}"
try 5 "int main(){ int a; int b; a = 5; b = 3; a;}"
try 5 "int main(){ int a; int b; int c; a = b = 3; a = 5; c = 2; a;}"
try 11 "int main(){ return 11;}"
try 14 "int main(){ int a; int b; a = 3; b = 5 * 6 - 8; return a + b / 2;}"
try 5 "int main(){ return 5; return 8;}"
try 23 "int main(){ int foo; foo = 23;}"
try 23 "int main(){ int foo; int bar; foo = 23; bar = 24; foo;}"
try 29 "int main(){ int foo; int bar; int buzz; foo = 23; bar = 29; buzz = 31; bar;}"
try 6 "int main(){ int foo; int bar; foo = 1; bar = 2 + 3; return foo + bar;}"
try 2 "int main(){ if (1) 2;}"
try 7 "int main(){ int a; a = 1; if (a == 1) a = 7; return a;}"
try 1 "int main(){ int a; a = 1; if (a != 1) a = 7; return a;}"
try 7 "int main(){ int a; a = 1; if (a == 1) a = 7; else a = 3; return a;}"
try 5 "int main(){ int a; a = 1; if (a != 1) a = 7; else a = 3; return a + 2;}"
try 5 "int main(){ int a; a = 1; if (a > 0) { if (a != 1) a = 7; else a = 3;} return a + 2;}"
try 10  "int main(){ int i; i = 0; while (i < 10) i = i + 1; return i;}"
try 10  "int main(){ int a; int i; a = 0; for (i = 0; i < 5; i = i + 1) a = a + i; return a;}"
try 120  "int main(){ int a; int i; a = 1; for (i = 5; i > 0; i = i - 1) a = a * i; return a;}"
try 10  "int main(){ int i;  i = 0; while (i < 10) { i = i + 1; } return i;}"
try 10  "int main(){ int i; int a; i = 0; a = 0; while (i < 10) { i = i + 1; a = i;} return a;}"
try 2 "int main(){ if (1) {return 2;}}"
try 8 "int main(){ int a; a = 1; if (a) {a = 7; a = 8;} return a;}"
try 120  "int main(){ int a; int i; a = 1; for (i = 5; i > 0; i = i - 1) {a = a * i;} return a;}"
try 120  "int main(){ int a; int val; int i; a = 1; val = 0; for (i = 5; i > 0; i = i - 1) {a = a * i; val = a;} return val;}"
try -out OK "int main(){ print_ok();}"
try -out 31 "int main(){ print_i(31);}"
try -out 7 "int main(){ print_add_i(3, 4);}"
try -out 21 "int main(){ print_sum_i(1, 2, 3, 4, 5, 6);}"
try -out OK "int main(){ print_ok(); }"
try -out 31 "int main(){ print_i(31); }"
try -out 7 "int main(){ print_add_i(3, 4); }"
try -out 21 "int main(){ print_sum_i(1, 2, 3, 4, 5, 6); }"
try -out OK "int def() { print_ok(); } int main(){ def(); }"
try -out 9 "int def() { return 9; } int main(){ int a; a = def(); print_i(a);}"
try -out 9 "int def(int x) { int a; a = x; print_i(a); } int main(){ def(9); }"
try -out 42 "int def(int x, int y) { int a; a = x + y; print_i(a); } int main(){ def(11, 31); }"
try -out 21 "int def(int a, int b, int c, int d, int e, int f) { int x; int y; x = a + b * c; y = d * e - f; print_add_i(x,  y);} int main(){ def(1, 2, 3, 4, 5, 6); }"
try -out 7 "int def(int x, int y) { return x + y; } int main(){ int a; a = def(3, 4); print_i(a);}"
try -out 5 "int fib(int n) { if (n==0) return 0; if (n==1) return 1; return fib(n-2) + fib(n-1); } int main(){ int a; a = fib(5); print_i(a);}"
try -out 10946 "int fib(int n) { if (n==0) return 0; if (n==1) return 1; return fib(n-2) + fib(n-1); } int main(){ int a; a = fib(21); print_i(a);}"
try -out 7 "int main(){ int *a; int b; b = 5; a = &b; *a = 7; print_i(b);}"
try -out 0 "int main(){ int *a; int b; b = 5; a = &b; *a = 0; print_i(b);}"
try -out 5 "int main(){ int *a; int b; int c; b = 5; a = &b; c = *a; print_i(c);}"
try -out 0 "int main(){ int *a; int b; int c; b = 5; a = &b; *a = 0; c = *a; print_i(c);}"
try -out 3 "int main() { int x; int *y; x = 3; y = &x; print_i(*y);}"
try 3 "int main() { int x; int *y; x = 3; y = &x; return *y;}"
try 3 "int main() { int **x; int *y; int z; x = &y; y = &z; **x = 3; return z;}"
try 5 "int main() { int **x; int *y; int z; x = &y; y = &z; z = 5; return **x;}"
try 7 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z;  z = &a; ***x = 7; return a;}"
try 11 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z;  z = &a; a = 11; return ***x;}"
try 13 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z;  z = &a; ***x = 11; **y = 13; return a;}"
try 1 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z; z = &a;  is_equal_ptrs(**x, z);}"
try 1 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z; z = &a;  is_equal_ptrs(*y, z);}"
try 1 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z; z = &a;  is_equal_ptrs(*y, &a);}"
try 1 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z; z = &a;  is_equal_ptrs(*x, y);}"
try 0 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z; z = &a;  is_equal_ptrs(***x, &a);}"
try 1 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z; z = &a;  is_equal_ptrs(***x, a);}"
try -out 3  "int def(int *a) { print_i(*a); } int main() { int x; int *y; x = 3; y = &x; def(y);}"
try -out 7  "int def(int *x, int *y) { int ret; ret = *x + *y; print_i(ret); } int main() { int *a; int *b; int c; int d; \
    a = &c; b = &d; *a = 3; *b = 4; def(a, b);}"
try -out 1 "int main() { int *p; int *q; alloc4(&p, 1, 2, 4, 8); q = p; print_i(*q);}"
try -out 4 "int main() { int *p; int *q; alloc4(&p, 1, 2, 4, 8); q = p + 2; print_i(*q);}"
try -out 2 "int main() { int *p; int *q; alloc4(&p, 1, 2, 4, 8); q = p + 3; q = q - 2; print_i(*q);}"
try 4 "int main() { int x; sizeof(x); }"
try 8 "int main() { int *y; sizeof(y); }"
try 4 "int main() { int x; sizeof(x + 3); }"
try 8 "int main() { int *y; sizeof(y + 3); }"
try 4 "int main() { int *y; sizeof(*y); }"
try 4 "int main() { int *y; sizeof(1); }"
try 4 "int main() { int *y; sizeof(sizeof(1)); }"
try 4 "int main() { sizeof(1 == 1); }"
try 4 "int main() { sizeof(1 != 1); }"
try 4 "int main() { sizeof(1 < 1); }"
try 4 "int main() { sizeof(1 <= 1); }"
try 4 "int main() { sizeof(1 > 1); }"
try 4 "int main() { sizeof(1 >= 1); }"
try 4 "int main() { sizeof(1 * 2); }"
try 4 "int main() { sizeof(4 / 2); }"
try 4 "int main() { sizeof(1 + 2); }"
try 4 "int main() { sizeof(3 - 1); }"
try 4 "int main() { int x; sizeof(x - 3); }"
try 8 "int main() { int *y; sizeof(y - 3); }"
try 8 "int main() { int *y; sizeof(&y); }"
try 4 "int main() { int x; sizeof(x + 3 + 4); }"
try 4 "int main() { sizeof(3 * (1 + 2)); }"
try 4 "int main() { sizeof((3 + 1) > 1); }"
try 4 "int main() { int *x; int y; x = &y; sizeof((*x + 3) > 4); }"
try 8 "int main() { int *x; sizeof((x + 3) + 4); }"
try 4 "int main() { int a; sizeof(a = 1); }"
try 4 "int main() { int a; int *b; b = &a; sizeof(*b = 1); }"
try 8 "int main() { int *b; sizeof(b = 1); }"
try -out OK "int main() { int a[10]; print_ok();}"
try -out 3 "int main() { int a[2]; *a = 3; int *p; p = a; print_i(*p);}"
try -out 3 "int main() { int a[2]; *a = 3; int *p; int b; b = 4; p = a; print_i(*p);}"
try 3 "int main() { int a[2]; *a = 1; *(a + 1) = 2; int *p; p = a; return *p + *(p + 1);}"
try -out 3 "int main() { int a[10]; *(a + 2) = 3; int *p; p = a + 2; print_i(*p);}"
try -out 7 "int main() { int a[10]; *(a + 5) = 7; print_i(*(a + 5));}"
try -out 1 "int main() { int a[21]; *a = 1; *(a + 1) = 2; int *p; p = a; print_i(*p);}"
try 3 "int main() { int a[10]; *a = 1; *(a + 1) = 2; int *p; p = a; return *p + *(p + 1);}"
try 5 "int main() { int a[100]; *a = 2; *(a + 11) = 3; int *p; p = a; return *p + *(p + 11);}"
try 1 "int main() { int a[2]; a[0] = 1; return a[0];}"
try 5 "int main() { int a[100]; a[0] = 2; a[11] = 3; return a[0] + a[11];}"
try 0 "int a; int main() { 0; }"
try 0 "int a; int main() { return a;}"
try 1 "int a; int main() { a = 1; return a;}"
try 3 "int foo; int bar; int main() { foo = 1; bar = 2; return foo + bar;}"
try 0 "int a[10]; int main() { return a[0];}"
try 2 "int a[10]; int main() { a[0] = 2; return a[0];}"
try 5 "int a[100]; int main() { a[0] = 2; a[11] = 3; return a[0] + a[11];}"
try 5 "int a[100]; int main() { int x; x = 11; a[0] = 2; a[x] = 3; return a[0] + a[x];}"
try 45 "int main() { int a[10]; int i; for (i=0; i<10; i=i+1) {a[i] = i;} return a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8] + a[9];}"
try 45 "int a[10]; int i; int main() { for (i=0; i<10; i=i+1) {a[i] = i;} return a[0] + a[1] + a[2] + a[3] + a[4] + a[5] + a[6] + a[7] + a[8] + a[9];}"
try 1 "char a; int main() { a = 1; return a;}"
try 1 "int main() { char x[10];  x[0] = -1; x[1] = 2; return x[0] + x[1];}"
try 1 "char x[10]; int main() { x[0] = -1; x[1] = 2; return x[0] + x[1];}"
try 3 "int main() { char x[3]; int y; x[0] = -1; x[1] = 2; y = 4; return x[0] + y;}"
try 3 "char x[3]; int y; int main() { x[0] = -1; x[1] = 2; y = 4; return x[0] + y;}"
try -out abcb 'int main() { char *x; x = "abcb"; printfoo(x);}'
try -out Hello 'int main() { char *y; y = "Hello"; printfoo(y);}'

#Test using Eight queens puzzle
try 92 -file "test/test_8queen.c"

echo OK
