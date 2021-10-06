#!/bin/bash

for file in file1.txt file2.txt file3.txt file4.txt 
do
	for buffer in 100 1000 10000 100000
	do
		for thread in 2 8 32 64
		do
			echo "Step5 $file $buffer $thread"
			time ./step5 $file $buffer $thread
		done
	done
done