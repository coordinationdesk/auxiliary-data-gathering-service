#!/bin/bash
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/components/ADGS/lib/"
/usr/local/components/ADGS/bin/MultiDownloadInventoryObject $*
