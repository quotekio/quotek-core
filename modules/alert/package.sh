#!/bin/bash

tar -cvf alert.tar libalert.so
cd install
tar -rvf ../alert.tar *
cd ..
gzip alert.tar

