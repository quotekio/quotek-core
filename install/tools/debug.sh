#!/bin/bash

export LD_LIBRARY_PATH=/usr/local/adam/lib


case $1 in

  genetics)
    gdb --args /usr/local/adam/bin/adam --genetics --backtest-speed 2000 --backtest-dump /data/2013/dumps/20130726.vdump
  ;;


esac
