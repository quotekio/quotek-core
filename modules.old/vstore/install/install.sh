#!/bin/sh

echo "* ADAM VSTORE MODULE INSTALL*"
echo "Adam Install Directory:"
read INSTALL_DIR

if ! [ -f $INSTALL_DIR/bin/adam ]
then
  echo "Adam not found in directory, aborting install"
  exit 1
fi

cp libvstore.so $INSTALL_DIR/lib/
cp dump2vstore $INSTALL_DIR/tools/
cp vstore2dump $INSTALL_DIR/tools/

#cp etc/alert.conf $INSTALL_DIR/etc/

