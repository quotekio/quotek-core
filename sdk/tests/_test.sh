#!/bin/bash

. testconf.sh

for i in `ls|grep test_`; do

  resp=""

  if [[ $i == "test_alchemy" &&  $ALCHEMY_KEY != "none" ]]
  
  then
    CMD="./$i $ALCHEMY_KEY"
  else
    CMD="./$i"
  fi

  $CMD 2>/dev/null && resp="OK" || resp="\e[31mERROR\e[0m"

  echo -e "Tested $i: [$resp]"


done
