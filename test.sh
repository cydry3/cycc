#!/bin/bash
try() {
    if [ "$1" = "-out" ]; then
	expected="$2"
	input="$3"
    else
	expected="$1"
	input="$2"
    fi

    ./cycc "$input" > tmp.s

    if [ "$1" = "-out" ]; then
	gcc -o tmp tmp.s foo.o
	actual=`./tmp`
    else
	gcc -o tmp tmp.s
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
try 10  "int main(){ int i; i = 0; while (i < 10) i = i + 1; return i;}"
try 10  "int main(){ int a; int i; a = 0; for (i = 0; i < 5; i = i + 1) a = a + i; return a;}"
try 120  "int main(){ int a; int i; a = 1; for (i = 5; i > 0; i = i - 1) a = a * i; return a;}"
try 10  "int main(){ int i;  i = 0; while (i < 10) { i = i + 1; } return i;}"
try 10  "int main(){ int i; int a; i = 0; a = 0; while (i < 10) { i = i + 1; a = i;} return a;}"
try 2 "int main(){ if (1) {return 2;}}"
try 8 "int main(){ int a; a = 1; if (a) {a = 7; a = 8;} return a;}"
try 120  "int main(){ int a; int i; a = 1; for (i = 5; i > 0; i = i - 1) {a = a * i;} return a;}"
try 120  "int main(){ int a; int val; int i; a = 1; val = 0; for (i = 5; i > 0; i = i - 1) {a = a * i; val = a;} return val;}"
try -out OK "int main(){ foo();}"
try -out 31 "int main(){ bar(31);}"
try -out 7 "int main(){ buzz(3, 4);}"
try -out 21 "int main(){ fizz(1, 2, 3, 4, 5, 6);}"
try -out OK "int main(){ foo(); }"
try -out 31 "int main(){ bar(31); }"
try -out 7 "int main(){ buzz(3, 4); }"
try -out 21 "int main(){ fizz(1, 2, 3, 4, 5, 6); }"
try -out OK "int def() { foo(); } int main(){ def(); }"
try -out 9 "int def() { return 9; } int main(){ int a; a = def(); bar(a);}"
try -out 9 "int def(int x) { int a; a = x; bar(a); } int main(){ def(9); }"
try -out 42 "int def(int x, int y) { int a; a = x + y; bar(a); } int main(){ def(11, 31); }"
try -out 21 "int def(int a, int b, int c, int d, int e, int f) { int x; int y; x = a + b * c; y = d * e - f; buzz(x,  y);} int main(){ def(1, 2, 3, 4, 5, 6); }"
try -out 7 "int def(int x, int y) { return x + y; } int main(){ int a; a = def(3, 4); bar(a);}"
try -out 5 "int fib(int n) { if (n==0) return 0; if (n==1) return 1; return fib(n-2) + fib(n-1); } int main(){ int a; a = fib(5); bar(a);}"
try -out 10946 "int fib(int n) { if (n==0) return 0; if (n==1) return 1; return fib(n-2) + fib(n-1); } int main(){ int a; a = fib(21); bar(a);}"
try -out 7 "int main(){ int *a; int b; b = 5; a = &b; *a = 7; bar(b);}"
try -out 0 "int main(){ int *a; int b; b = 5; a = &b; *a = 0; bar(b);}"
try -out 5 "int main(){ int *a; int b; int c; b = 5; a = &b; c = *a; bar(c);}"
try -out 0 "int main(){ int *a; int b; int c; b = 5; a = &b; *a = 0; c = *a; bar(c);}"
try -out 3 "int main() { int x; int *y; x = 3; y = &x; bar(*y);}"
try 3 "int main() { int x; int *y; x = 3; y = &x; return *y;}"
try 3 "int main() { int **x; int *y; int z; x = &y; y = &z; **x = 3; return z;}"
try 5 "int main() { int **x; int *y; int z; x = &y; y = &z; z = 5; return **x;}"
try 7 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z;  z = &a; ***x = 7; return a;}"
try 11 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z;  z = &a; a = 11; return ***x;}"
try 13 "int main() { int ***x; int **y; int *z; int a; x = &y; y = &z;  z = &a; ***x = 11; **y = 13; return a;}"


echo OK
