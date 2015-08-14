#!/bin/bash

if [ -d ../doc/html ]; then
 rm -rf ../doc/html
fi

if [ -d ../doc/xml ]; then
 rm -rf ../doc/xml
fi


doxygen sdk2.doxy
cp ext/*.css ../doc/html/
cp ext/*.js ../doc/html/


