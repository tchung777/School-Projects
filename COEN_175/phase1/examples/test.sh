#!/bin/bash

array=("tricky" "hello" "fib" "list" "malloc" "sum")

for i in "${array[@]}"
do
	echo $i
	./scc < $i.c > $i.txt
	diff $i.txt $i.out
done