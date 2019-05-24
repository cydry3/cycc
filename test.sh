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

try 0 "main(){ 0; }"
try 42 "main(){ 42;}"
try 21 "main(){ 5+20-4;}"
try 41 "main(){  12 + 34 - 5 ;}"
try 47 "main(){ 5+6*7;}"
try 15 "main(){ 5*(9-6);}"
try 4 "main(){ (3+5)/2;}"
try 20 "main(){ -25+45;}"
try 68 "main(){ 93+(-25);}"
try 1 "main(){ 1==1;}"
try 0 "main(){ 1==0;}"
try 1 "main(){ 1!=0;}"
try 0 "main(){ 1!=1;}"
try 0 "main(){ 1<=0;}"
try 1 "main(){ 1<=1;}"
try 1 "main(){ 0<=1;}"
try 1 "main(){ 1>=0;}"
try 1 "main(){ 1>=1;}"
try 0 "main(){ 0>=1;}"
try 0 "main(){ 0>=1;}"
try 0 "main(){ 1<1;}"
try 1 "main(){ 0<1;}"
try 0 "main(){ 1<0;}"
try 0 "main(){ 1>1;}"
try 0 "main(){ 0>1;}"
try 1 "main(){ 1>0;}"
try 50 "main(){ 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;}"
try 51 "main(){ 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;}"
try 100 "main(){ 1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;}"
try 5 "main(){ int a; a = 5;}"
try 2 "main(){ int a; int b; a = b = 2;}"
try 2 "main(){ int a; int b; int c; a = b = 3; c = 2;}"
try 5 "main(){ int a; int b; a = 5; b = 3; a;}"
try 5 "main(){ int a; int b; int c; a = b = 3; a = 5; c = 2; a;}"
try 11 "main(){ return 11;}"
try 14 "main(){ int a; int b; a = 3; b = 5 * 6 - 8; return a + b / 2;}"
try 5 "main(){ return 5; return 8;}"
try 23 "main(){ int foo; foo = 23;}"
try 23 "main(){ int foo; int bar; foo = 23; bar = 24; foo;}"
try 29 "main(){ int foo; int bar; int buzz; foo = 23; bar = 29; buzz = 31; bar;}"
try 6 "main(){ int foo; int bar; foo = 1; bar = 2 + 3; return foo + bar;}"
try 2 "main(){ if (1) 2;}"
try 7 "main(){ int a; a = 1; if (a == 1) a = 7; return a;}"
try 1 "main(){ int a; a = 1; if (a != 1) a = 7; return a;}"
try 7 "main(){ int a; a = 1; if (a == 1) a = 7; else a = 3; return a;}"
try 5 "main(){ int a; a = 1; if (a != 1) a = 7; else a = 3; return a + 2;}"
try 10  "main(){ int i; i = 0; while (i < 10) i = i + 1; return i;}"
try 10  "main(){ int a; int i; a = 0; for (i = 0; i < 5; i = i + 1) a = a + i; return a;}"
try 120  "main(){ int a; int i; a = 1; for (i = 5; i > 0; i = i - 1) a = a * i; return a;}"
try 10  "main(){ int i;  i = 0; while (i < 10) { i = i + 1; } return i;}"
try 10  "main(){ int i; int a; i = 0; a = 0; while (i < 10) { i = i + 1; a = i;} return a;}"
try 2 "main(){ if (1) {return 2;}}"
try 8 "main(){ int a; a = 1; if (a) {a = 7; a = 8;} return a;}"
try 120  "main(){ int a; int i; a = 1; for (i = 5; i > 0; i = i - 1) {a = a * i;} return a;}"
try 120  "main(){ int a; int val; int i; a = 1; val = 0; for (i = 5; i > 0; i = i - 1) {a = a * i; val = a;} return val;}"
try -out OK "main(){ foo();}"
try -out 31 "main(){ bar(31);}"
try -out 7 "main(){ buzz(3, 4);}"
try -out 21 "main(){ fizz(1, 2, 3, 4, 5, 6);}"
try -out OK "main(){ foo(); }"
try -out 31 "main(){ bar(31); }"
try -out 7 "main(){ buzz(3, 4); }"
try -out 21 "main(){ fizz(1, 2, 3, 4, 5, 6); }"
try -out OK "def() { foo(); } main(){ def(); }"
try -out 9 "def() { return 9; } main(){ a = def(); bar(a);}"
try -out 9 "def(x) { a = x; bar(a); } main(){ def(9); }"
try -out 42 "def(x, y) { a = x + y; bar(a); } main(){ def(11, 31); }"
try -out 21 "def(a,b,c,d,e,f) { x = a + b * c; y = d * e - f; buzz(x, y);} main(){ def(1,2,3,4,5,6); }"
try -out 7 "def(x, y) { return x + y; } main(){ a = def(3, 4); bar(a);}"
try -out 5 "fib(n) { if (n==0) return 0; if (n==1) return 1; return fib(n-2) + fib(n-1); } main(){ a = fib(5); bar(a);}"
try -out 10946 "fib(n) { if (n==0) return 0; if (n==1) return 1; return fib(n-2) + fib(n-1); } main(){ a = fib(21); bar(a);}"


echo OK
