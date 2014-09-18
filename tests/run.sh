#!/bin/sh

for i in `ls|grep test_|egrep -v '(\.cpp|\.c)'`
do

  ./$i

done

