#!/bin/bash

make clean
if  [ -f install_manifest.txt ] ;then 
  rm install_manifest.txt
fi

rm CMakeCache.txt
rm -rf CMakeFiles
rm cmake_install.cmake
rm Makefile

