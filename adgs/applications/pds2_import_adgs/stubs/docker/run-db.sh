#!/bin/bash

# 1) pg_dump -s -C -U pdsuser -h adgs-db1 adgsdb > '02 - adgsdb.sql'
# 2) change database create command to
#    CREATE DATABASE adgsdb WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';
# 3) comment line 
#    --SET default_table_access_method = heap;
# 4) perform a "copy <table> to stdout;" command for each of the data tables needed and
#    copy contents to the "data/<db>" folder in a separate file


SQL_DIR=$PWD/db/
if [ -z "$1" ]; then
	DATA_DIR=$SQL_DIR/data/default
else
	DATA_DIR=$SQL_DIR/data/$1
fi

if [ ! -d $DATA_DIR ]; then
	echo "not a directory: $DATA_DIR"
	exit 1
fi

DB_NAME=adgsdb
TMP_DIR=/tmp/$DB_NAME.$$
mkdir -p $TMP_DIR

cp $SQL_DIR/*.sql $TMP_DIR
cp $DATA_DIR/*.sql $TMP_DIR

docker run \
	--rm \
	-p 5432:5432 \
	-v $TMP_DIR:/docker-entrypoint-initdb.d/ \
	--tmpfs /var/lib/postgresql/data \
	--name=$DB_NAME mdillon/postgis
