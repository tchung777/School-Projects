#!/bin/bash

# Created by Joseph Phan
# To use: make sure you have an "examples" directory in the directory with the executable ./scc

for file in examples/*.c;
do
    filename=${file%.c}
    echo "------------------------"
    echo "Testing Against: ${filename}"
    cp ${filename}.c test.c
    cp ${filename}.s test.s
    ./scc < test.c > myTest.s
	diff myTest.s test.s
done
rm test.c
rm test.s
rm myTest.s
