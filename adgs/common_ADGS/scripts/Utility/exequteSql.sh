#!/bin/bash

export PGPASSWORD=estestest

# $1 -> database hostname
# $2 -> sql file
# $3 -> database name

psql -U acsdba -h $1 -f $2 $3
