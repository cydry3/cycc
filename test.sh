#!/bin/bash
try() {
    expected="$1"
    input="$2"

    ./cycc "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
	echo "$input => $actual"
    else
	echo "$input => $actual"
	echo "$expected expected, but got $actual"
	exit 1
    fi
}

try 0 "0;"
try 42 "42;"
try 21 "5+20-4;"
try 41 " 12 + 34 - 5 ;"
try 47 "5+6*7;"
try 15 "5*(9-6);"
try 4 "(3+5)/2;"
try 20 "-25+45;"
try 68 "93+(-25);"
try 1 "1==1;"
try 0 "1==0;"
try 1 "1!=0;"
try 0 "1!=1;"
try 0 "1<=0;"
try 1 "1<=1;"
try 1 "0<=1;"
try 1 "1>=0;"
try 1 "1>=1;"
try 0 "0>=1;"
try 0 "0>=1;"
try 0 "1<1;"
try 1 "0<1;"
try 0 "1<0;"
try 0 "1>1;"
try 0 "0>1;"
try 1 "1>0;"
try 50 "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;"
try 51 "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;"
try 100 "1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1;"
try 5 "a = 5;"
try 2 "a = b = 2;"
try 2 "a = b = 3; c = 2;"
try 5 "a = 5; b = 3; a;"
try 5 "a = b = 3; a = 5; c = 2; a;"
try 11 "return 11;"
try 14 "a = 3; b = 5 * 6 - 8; return a + b / 2;"
try 5 "return 5; return 8;"
try 23 "foo = 23;"
try 23 "foo = 23; bar = 24; foo;"
try 29 "foo = 23; bar = 29; buzz = 31; bar;"
try 6 "foo = 1; bar = 2 + 3; return foo + bar;"
try 2 "if (1) 2;"
try 7 "a = 1; if (a == 1) a = 7; return a;"
try 1 "a = 1; if (a != 1) a = 7; return a;"
try 7 "a = 1; if (a == 1) a = 7; else a = 3; return a;"
try 5 "a = 1; if (a != 1) a = 7; else a = 3; return a + 2;"
try 10  "i = 0; while (i < 10) i = i + 1; return i;"
try 10  "a = 0; for (i = 0; i < 5; i = i + 1) a = a + i; return a;"
try 120  "a = 1; for (i = 5; i > 0; i = i - 1) a = a * i; return a;"
try 10  "i = 0; while (i < 10) { i = i + 1; } return i;"
try 10  "i = 0; a = 0; while (i < 10) { i = i + 1; a = i;} return a;"
try 2 "if (1) {return 2;}"
try 8 "a = 1; if (a) {a = 7; a = 8;} return a;"
try 120  "a = 1; for (i = 5; i > 0; i = i - 1) {a = a * i;} return a;"
try 120  "a = 1; val = 0; for (i = 5; i > 0; i = i - 1) {a = a * i; val = a;} return val;"

echo OK
