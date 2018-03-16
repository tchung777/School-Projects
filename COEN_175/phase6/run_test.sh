#!/bin/bash

make

###### Test Everything #######
for file in examples/*.c;
do
    filename=${file%.c}
    echo $filename
    # if [ ${filename} == "examples/calc" ]
    # then
    #     continue
    # fi
    ./scc < ${filename}.c > ${filename}.s
    gcc -m32 ${filename}.s
    ./a.out < ${filename}.in > ${filename}.txt
    echo " --------- DIFF  ${filename}"
    diff ${filename}.out ${filename}.txt
    echo " --------- END   DIFFS"
done


######## Single Test #########
# file="examples/matrix.c"
# filename=${file%.c}
# ./scc < ${filename}.c > ${filename}.s
# gcc -m32 ${filename}.s
# ./a.out < ${filename}.in > ${filename}.txt
# echo " --------- DIFF  ${filename}"
# diff ${filename}.out ${filename}.txt
# echo " --------- END   DIFFS"


make clobber
make clean
rm examples/*.txt
rm examples/*.s
rm a.out