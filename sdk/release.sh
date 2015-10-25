#!/bin/bash

### BUILD ###
./clean.sh
cp CMakeLists.txt CMakeLists.txt.bak
sed -i 's/-stdlib=libc++//' CMakeLists.txt
cp CMakeLists.txt CMakeLists.txt.rel
cmake .
sudo make install && rm CMakeLists.txt.rel
mv CMakeLists.txt.bak CMakeLists.txt

### DOCUMENTATION BUILD ###

cd doxygen
./makedoc.sh
cd breathe
make clean
make html
cd ../../

### PACKAGING TO GITHUB REPOS ###

git clone git@github.com:quotekio/quotek-sdk.git ./quotek-sdk-$1
cd ./quotek-sdk-$1 || exit 1
rm -rf ./include/quotek
rm -rf ./doc/html
rm -rf ./samples
rm -rf ./tests
cp -r /usr/local/include/quotek ./include/
cp -r ../tests ./
cp -r ../samples ./
cp -r ../doxygen/breathe/build/html ./doc/
cp /usr/local/lib/libquotek.a ./lib/linux/
cd ..

