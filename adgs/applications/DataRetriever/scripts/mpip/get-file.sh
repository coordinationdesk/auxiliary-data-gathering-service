#!/bin/bash -e

. request.sh

if [ -z "$1" ]; then
	echo "usage: $0 json_filter"
	echo "for example: $0 '{\"validityStartTime\": \"2022-01-05T16:00:00.000\", \"validityStopTime\": \"2022-05-17T18:00:00.000\"}'"
	exit 1
fi

mpipRequest '/file' "$1"

