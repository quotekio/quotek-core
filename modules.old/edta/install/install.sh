#!/bin/sh

echo "* ADAM ALERT MODULE INSTALL*"
echo "Adam Install Directory:"
read INSTALL_DIR

if ! [ -f $INSTALL_DIR/bin/adam ]
then
  echo "Adam not found in directory, aborting install"
  exit 1
fi

cp libalert.so $INSTALL_DIR/lib/
cp etc/alert.conf $INSTALL_DIR/etc/
cp compile/headers/* $INSTALL_DIR/compile/headers/
cp compile/obj/* $INSTALL_DIR/compile/obj/

HAS_ALERT_H=`cat $INSTALL_DIR/compile/headers/strats.h| grep -o alert.h`
if ! [ "${HAS_ALERT_H}" = "alert.h" ]
then
  sed -i 's/\/\/\!MH_APPEND\!/#include "alert.h"\n\/\/!MH_APPEND!\n/' $INSTALL_DIR/compile/headers/strats.h
fi

