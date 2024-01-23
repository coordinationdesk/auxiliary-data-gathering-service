#!/bin/bash



# debugging routine: prints all arguments if the $GETMODULEVERSION_DEBUG variable is defined
debug() {
    if [ "$CURL_ENUM_DEBUG" ]
    then
    	echo DEBUGGING "$*" 1>&2
    fi
}

#curlh=$(rpm -ql $(rpm -qa | grep curl | grep devel) | grep curl.h)
curlh=/usr/include/curl/curl.h
debug curlh: ${curlh}

echo 'List of CURL errors codes:'
egrep "CURLE_[A-Z_0-9 \t]+\," ${curlh} | sed "s/^[ \t]*/\"/" | sed "s/,[ \t]*\/\*/\",/" | tr -d " \t" | egrep -o "^.*[0-9]" | awk -F',' 'OFS="," {print "{" $2, $1 "}," }'

#echo 'List of CURL errors codes aliases:'
#egrep "CURLE_[A-Z_]+" ${curlh} | grep define | awk -F' ' 'OFS="," {print "{" "\""$2"\",\"" $3 "\"}," }'
#exit 0 
