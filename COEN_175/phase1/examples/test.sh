#!/bin/bash

lex -t lexer.l > lexer.cpp
g++ -o scc lexer.cpp

array=("tricky" "hello" "fib" "list" "malloc" "sum")

for i in "${array[@]}"
do
	echo $i
	./scc < $i.c > $i.txt
	diff $i.txt $i.out
done