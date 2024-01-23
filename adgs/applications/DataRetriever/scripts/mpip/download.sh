#!/bin/bash -e

. request.sh

if [ -z "$1" ] ; then
	echo "usage: $0 filename"
	exit 1
fi

mpipRequest "/download?filename=$1"

