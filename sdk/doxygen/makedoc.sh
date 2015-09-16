#!/bin/bash

if [ -d ../doc/xml ]; then
 rm -rf ../doc/xml
fi

doxygen sdk2.doxy
