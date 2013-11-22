#!/bin/bash

for i in `ls -1 *.c`
do
  gcc -c -shared -fPIC -rdynamic $i
done

#for i in `ls -1 *.cpp`
#do
#  g++ -c -shared -fPIC -rdynamic $i
#done
