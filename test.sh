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
try 5 "main(){ a = 5;}"
try 2 "main(){ a = b = 2;}"
try 2 "main(){ a = b = 3; c = 2;}"
try 5 "main(){ a = 5; b = 3; a;}"
try 5 "main(){ a = b = 3; a = 5; c = 2; a;}"
try 11 "main(){ return 11;}"
try 14 "main(){ a = 3; b = 5 * 6 - 8; return a + b / 2;}"
try 5 "main(){ return 5; return 8;}"
try 23 "main(){ foo = 23;}"
try 23 "main(){ foo = 23; bar = 24; foo;}"
try 29 "main(){ foo = 23; bar = 29; buzz = 31; bar;}"
try 6 "main(){ foo = 1; bar = 2 + 3; return foo + bar;}"
try 2 "main(){ if (1) 2;}"
try 7 "main(){ a = 1; if (a == 1) a = 7; return a;}"
try 1 "main(){ a = 1; if (a != 1) a = 7; return a;}"
try 7 "main(){ a = 1; if (a == 1) a = 7; else a = 3; return a;}"
try 5 "main(){ a = 1; if (a != 1) a = 7; else a = 3; return a + 2;}"
try 10  "main(){ i = 0; while (i < 10) i = i + 1; return i;}"
try 10  "main(){ a = 0; for (i = 0; i < 5; i = i + 1) a = a + i; return a;}"
try 120  "main(){ a = 1; for (i = 5; i > 0; i = i - 1) a = a * i; return a;}"
try 10  "main(){ i = 0; while (i < 10) { i = i + 1; } return i;}"
try 10  "main(){ i = 0; a = 0; while (i < 10) { i = i + 1; a = i;} return a;}"
try 2 "main(){ if (1) {return 2;}}"
try 8 "main(){ a = 1; if (a) {a = 7; a = 8;} return a;}"
try 120  "main(){ a = 1; for (i = 5; i > 0; i = i - 1) {a = a * i;} return a;}"
try 120  "main(){ a = 1; val = 0; for (i = 5; i > 0; i = i - 1) {a = a * i; val = a;} return val;}"
try -out OK "main(){ foo();}"
try -out 31 "main(){ bar(31);}"
try -out 7 "main(){ buzz(3, 4);}"
try -out 21 "main(){ fizz(1, 2, 3, 4, 5, 6);}"
try -out OK "main(){ foo(); }"
try -out 31 "main(){ bar(31); }"
try -out 7 "main(){ buzz(3, 4); }"
try -out 21 "main(){ fizz(1, 2, 3, 4, 5, 6); }"
try -out OK "def() { foo(); } main(){ def(); }"

echo OK
