#!/bin/bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/components/ADGS/lib/"
/usr/local/components/ADGS/bin/DownloadInventoryFile -f $1 -V $2 -d $3
if [[ $3 =~ "tmp" ]]; then
	test -e $3/$1 && chmod -R a+rwx $3/$1
fi

