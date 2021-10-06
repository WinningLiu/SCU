#!/bin/bash

for file in file1.txt file2.txt file3.txt file4.txt file5.txt
do
    for buffer in 100 1000 10000 100000
    do
        echo "Step3 $file $buffer"
        time ./step3 $file $buffer
        echo " "
    done
done