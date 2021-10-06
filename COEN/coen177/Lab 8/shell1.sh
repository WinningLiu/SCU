#!/bin/bash

make;
echo "----------FIFO----------"
cat accesses.txt | ./pageReplacement 50 1
echo "----------End FIFO----------"
echo
echo "----------LRU----------"
cat accesses.txt | ./pageReplacement 50 2
echo "----------End LRU----------"
echo
echo "----------Second Chance----------"
cat accesses.txt | ./pageReplacement 50 3
echo "----------End Second Chance----------"
echo

echo "FIFO accesses test with cache size = 10, 50, 100, 250, 500"
cat accesses.txt | ./pageReplacement 10 1
cat accesses.txt | ./pageReplacement 50 1
cat accesses.txt | ./pageReplacement 100 1
cat accesses.txt | ./pageReplacement 250 1
cat accesses.txt | ./pageReplacement 500 1
echo
echo "LRU accesses test with cache size = 10, 50, 100, 250, 500"
cat accesses.txt | ./pageReplacement 10 2
cat accesses.txt | ./pageReplacement 50 2
cat accesses.txt | ./pageReplacement 100 2
cat accesses.txt | ./pageReplacement 250 2
cat accesses.txt | ./pageReplacement 500 2
echo
echo "Second Chance accesses test with cache size = 10, 50, 100, 250, 500"
cat accesses.txt | ./pageReplacement 10 3
cat accesses.txt | ./pageReplacement 50 3
cat accesses.txt | ./pageReplacement 100 3
cat accesses.txt | ./pageReplacement 250 3
cat accesses.txt | ./pageReplacement 500 3
echo

make clean;
echo
