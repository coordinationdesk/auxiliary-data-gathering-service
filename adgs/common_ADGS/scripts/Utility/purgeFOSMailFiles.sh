#!/bin/bash
#
# This script deletes the file sent by FOS not used by ADGS older than 5 days
#
for i in `find /data/fromFOSMail -type f -mtime +5`
do
        rm -f $i
done
