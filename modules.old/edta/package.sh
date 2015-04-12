#!/bin/bash

tar -cvf edta.tar libedta.so
cd install
tar -rvf ../alert.tar *
cd ..
gzip edta.tar

