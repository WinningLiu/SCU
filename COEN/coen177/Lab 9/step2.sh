#!/bin/bash

for file in file1.txt file2.txt file3.txt file4.txt file5.txt
do     
    echo "Step2 $file"
    time ./step2 $file
    echo " "
done