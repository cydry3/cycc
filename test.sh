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
try 0 -file "tests/test_ident.c"
try 0 -file "tests/test_ptr.c"
try 0 -file "tests/test_array.c"
try 0 -file "tests/test_expr.c"
try 0 -file "tests/test_stmt.c"
try 0 -file "tests/test_ext_def.c"
try 0 -file "tests/test_comment.c"

#Test using stdout
try -out abcb 'int main() { char *x; x = "abcb"; printfoo(x);}'
try -out Hello 'int main() { char *y; y = "Hello"; printfoo(y);}'

#Test using Eight queens puzzle
try 92 -file "tests/test_8queen.c"

echo OK
