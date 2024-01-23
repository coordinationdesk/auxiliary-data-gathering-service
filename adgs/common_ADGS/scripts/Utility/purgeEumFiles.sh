#!/bin/bash
#
# This script deletes the uneuseful ADF files sent by EUM older than 5 days
#
for i in `find /data/fromEUM -name "S3*" -mtime +5`
do
        rm -fr $i
done

