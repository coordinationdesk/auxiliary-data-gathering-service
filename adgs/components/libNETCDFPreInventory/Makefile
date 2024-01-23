#
#	Copyright 1995-2012, Advanced Computer Systems , Inc.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.acsys.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Advanced Computer Systems, Inc.
#
#	$Prod: A.C.S. libNETCDFPreInventory$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.5  2013/01/23 16:04:08  marpas
#	fixed
#	
#	Revision 1.4  2012/10/29 15:01:15  giufor
#	inserted in DSODEPENDS library netcdf, hdf5 and curl
#	
#	Revision 1.3  2012/10/29 13:23:13  giufor
#	updated
#	
#	Revision 1.2  2012/10/25 14:56:58  giufor
#	updated
#	
#	Revision 1.1.1.1  2012/10/22 10:25:47  giufor
#	Import libNETCDFPreInventory
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libNETCDFPreInventory

VERSIONFILE = NETCDFPreInventoryV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = NETCDFPreInventory.C \
	NETCDFPreInventoryFactory.C \
	NETCDFPreInventoryV.C

C++INCLUDE = NETCDFPreInventory.h \
	NETCDFPreInventoryFactory.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOSPECIFICDEPS = \
        -lnetcdf_c++4 \
        -lnetcdf \
        -lhdf5_hl \
        -lhdf5 

# DO NOT DELETE

include ../../common/commonrules.mk
