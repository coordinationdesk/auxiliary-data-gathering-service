#!/bin/bash
#
# This script deletes the lock files generated by the report generation older than 36 hours
#

for i in `find /data/ADGS/report -type f -name "*.lock" -mtime +1.5`
do
	rm -f $i
done
