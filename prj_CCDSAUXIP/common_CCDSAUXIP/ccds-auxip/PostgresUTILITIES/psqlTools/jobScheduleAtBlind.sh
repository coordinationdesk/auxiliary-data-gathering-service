#!/bin/sh
#
# Copyright 2010-2020, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.
# 
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, S.p.A.
#	
#
#   $Prod: A.C.S. Run jobs during blind orbits $
#
#	$Id$
#
#	$Author$
#
#	$Log$








export EXE_DIR=$(dirname $0)
cd $EXE_DIR
export EXE_DIR=$(pwd)
export JOB=$EXE_DIR/jobScheduleAtBlind.job

at -f $JOB now + 1141 min

export PGPASSWORD=estestest
$EXE_DIR/vacuumTables localhost acsdba cryosat2 --pds
$EXE_DIR/reindexTables localhost acsdba cryosat2 --pds
$EXE_DIR/analyzeTables localhost acsdba cryosat2 --pds




