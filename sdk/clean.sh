#!/bin/bash

if [ -f Makefile ] ;then
  make clean
  rm Makefile
fi

rm CMakeCache.txt
rm -rf CMakeFiles
rm cmake_install.cmake
rm install_manifest.txt
