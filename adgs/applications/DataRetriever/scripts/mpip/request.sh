#!/bin/bash

API_HOST=api.s12mpip.com
API_URL="https://$API_HOST/mpip"
#API_URL='https://localhost:1443/mpip'

function getAuthHeader() {
	if [ ! -f access-token.txt ]; then
		echo "access token file does not exist"
		exit 1
	fi
	echo "Authorization: Bearer $(cat access-token.txt)"
}

# $1 = request path
# $2 = [optional] request body
function mpipRequest() {
	local AUTH_HEADER=$(getAuthHeader)
	local REQUEST="$1"
	if [ ! -z "$2" ]; then
		local BODY="$2"
		local CONTENT_TYPE="application/json"
		curl -v -k -H "Host: $API_HOST" -H "$AUTH_HEADER" -H "Content-Type: $CONTENT_TYPE" -d "$BODY" "$API_URL$REQUEST"
	else
		local BODY=""
		local CONTENT_TYPE=""
		curl -v -k -H "Host: $API_HOST" -H "$AUTH_HEADER" "$API_URL$REQUEST"
	fi

}

