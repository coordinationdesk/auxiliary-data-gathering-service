#!/bin/bash
#
# Copyright 1995-2006, Advanced Computer Systems , Inc.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.acsys.it
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Advanced Computer Systems, Inc.
#
# $Prod: A.C.S. include symlinks generator $
#
# $Id$
# 
# $Author$
#
# $Log$
# Revision 1.8  2007/09/19 16:02:55  enrcar
# EC:: Mss_Structures support added. (Partial).
#
# Revision 1.7  2007/09/05 14:30:03  enrcar
# Bug fixed: StationManagment directory handled
#
# Revision 1.6  2007/09/05 14:23:04  enrcar
# landsat directory added
#
# Revision 1.5  2007/01/26 15:37:28  enrcar
# LAND_Structures added
#
# Revision 1.4  2007/01/26 15:31:52  enrcar
# added Tm_Structures.h
#
# Revision 1.3  2007/01/26 13:09:38  enrcar
# File ETM+_Structures.h added
#
# Revision 1.2  2006/09/27 16:05:26  enrcar
# unuseful messages removed
#
# Revision 1.1  2006/09/27 09:06:15  enrcar
# imported source
#
#



if [ "${LIB_INTEGRATION_PATH}" == "" ]; then

    echo "Env variable LIB_INTEGRATION_PATH not defined. Aborted." ;
    exit 1;

fi

INCLUDE_DIR=$LIB_INTEGRATION_PATH/include


mkdir -p $INCLUDE_DIR/LANDSAT $INCLUDE_DIR/JERS $INCLUDE_DIR/landsat $INCLUDE_DIR/StationManagement $INCLUDE_DIR/SPOT $INCLUDE_DIR/QuickLook $INCLUDE_DIR/DataTranscription

pushd $INCLUDE_DIR/LANDSAT > /dev/null
ln -sf ../ETM+_Structures.h .
ln -sf ../Tm_Structures.h .
ln -sf ../LAND_Structures.h .
ln -sf ../Mss_Structures.h .
popd > /dev/null

pushd $INCLUDE_DIR/JERS > /dev/null
ln -sf ../JErs_Ops_Structures.h .
ln -sf ../opsMagic.h .
popd > /dev/null

pushd $INCLUDE_DIR/landsat > /dev/null
ln -sf ../lsMagic.h .
ln -sf ../lsTypes.h .
popd > /dev/null

pushd $INCLUDE_DIR/StationManagement > /dev/null
ln -sf ../smTypes.h .
popd > /dev/null

pushd $INCLUDE_DIR/SPOT > /dev/null
ln -sf ../Spot4.h .
ln -sf ../Spot.h .
popd > /dev/null

pushd $INCLUDE_DIR/QuickLook > /dev/null
ln -sf ../Quick_Look.h .
popd > /dev/null

pushd $INCLUDE_DIR/DataTranscription > /dev/null
ln -sf ../dtMagic.h .
ln -sf ../dtTypes.h .
popd > /dev/null


exit 0 ;

