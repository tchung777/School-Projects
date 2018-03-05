#!/bin/bash

for file in examples/*.c;
do
  filename=${file%.c}
  b='-lib'
  if [[ ${filename} != *"-lib"* ]]
  then
    echo "------------------------"
    echo "Testing Against: ${filename}"
    cp ${filename}.c test.c
    cp ${filename}$b.c testlib.c
    cp ${filename}.out test.out
    ./scc < test.c > myTest.s
    gcc -m32 test.s testlib.c
    ./a.out > myTest.out
    diff test.out myTest.out
  fi
done

: '
do
    filename=${file%.c}
    echo "------------------------"
    echo "Testing Against: ${filename}"
    cp ${filename}.c test.c
    cp ${filename}.s test.s
    ./scc < test.c > myTest.s
    gcc -m32 test.s
	diff myTest.s test.s
done
rm test.c
rm test.s
rm myTest.s
'
