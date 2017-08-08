#!/bin/bash
#Creates database and default user

QSQL="CREATE DATABASE quotek;
CREATE ROLE quotek WITH LOGIN PASSWORD 'QPASS';"

QPASS=`head -n 100 /dev/urandom|md5sum|head -c10`

echo "$QSQL" > /tmp/quotek_initsql.sql

sed s/QPASS/$QPASS/ -i /tmp/quotek_initsql.sql
sudo -u postgres -s eval "psql < /tmp/quotek_initsql.sql"

echo "********************************************"
echo "DB PASSWORD FOR USER QUOTEK: $QPASS"
echo "********************************************"

echo $QPASS > /tmp/quotek_dbpasswd