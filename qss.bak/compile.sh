#!/bin/bash

for i in `ls -1 *.c`
do
  clang -c -fPIC $i
done

for i in `ls -1 *.cpp`
do
  clang++ -c -fPIC $i
done
