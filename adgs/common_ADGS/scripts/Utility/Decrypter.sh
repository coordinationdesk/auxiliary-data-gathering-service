#!/bin/bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/components/ADGS/lib/"
/usr/local/components/ADGS/bin/RemoteConnectionsKey_Decode $1 -m -n 
