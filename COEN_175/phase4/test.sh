#!/bin/bash

# Created by Joseph Phan
# To use: make sure you have an "examples" directory in the directory with the executable ./scc

for file in examples/*.c;
do
    filename=${file%.c}
    echo "------------------------"
    echo "Testing Against: ${filename}"
    cp ${filename}.c test.c
    cp ${filename}.err test.err
    ./scc < test.c 2> myTest.err
	diff myTest.err test.err
done
rm test.c
rm test.err
rm myTest.err
