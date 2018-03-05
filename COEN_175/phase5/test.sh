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
    if [ testlib.c ]; then
      gcc -m32 -w myTest.s testlib.c
    else
      gcc -m32 -w myTest.s
    fi
    ./a.out > myTest.out
    diff test.out myTest.out

    rm test.c
    rm testlib.c
    rm test.out
    rm myTest.s
    rm myTest.out
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
