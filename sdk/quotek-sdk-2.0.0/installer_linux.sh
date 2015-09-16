#!/bin/bash

echo "Quotek SDK 2.0.0 Installer"
echo "=========================="

if [ ! -d /opt/quotek ]; then
  mkdir /opt/quotek
fi

if [ ! -d /opt/quotek/include ]; then
  mkdir /opt/quotek/include
fi

if [ ! -d /opt/quotek/lib ]; then
  mkdir /opt/quotek/lib
fi

if [ ! -d /opt/quotek/doc ]; then
  mkdir /opt/quotek/doc
fi

cp -r lib/linux/* /opt/quotek/lib/
cp -r include/* /opt/quotek/include/
cp -r doc/* /opt/quotek/doc/
