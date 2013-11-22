#!/bin/bash

tar -cvf vstore.tar libvstore.so vstore2dump dump2vstore
cd install
tar -rvf ../vstore.tar *
cd ..
gzip vstore.tar

