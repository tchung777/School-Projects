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
    if [ $? -ne 0 ]; then
      cp ${filename}.out test.out
      ./scc < test.c > myTest.s
      gcc -m32 -w myTest.s testlib.c
      ./a.out > myTest.out
      diff test.out myTest.out
    else
      cp ${filename}.out test.out
      ./scc < test.c > myTest.s
      gcc -m32 -w myTest.s
      ./a.out > myTest.out
      diff test.out myTest.out
    fi
  fi
done
rm test.c
rm testlib.c
rm test.out
rm myTest.s
rm myTest.out

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
