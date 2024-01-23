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
#	$Prod: A.C.S. libEOFPreInventory$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.2  2012/10/22 15:19:40  giufor
#	updated
#	
#	Revision 1.1.1.1  2012/10/18 15:07:33  giufor
#	Import libGRIBPreInventory
#	
#	Revision 1.2  2012/10/11 10:39:09  giufor
#	updated
#	
#	Revision 1.1.1.1  2012/10/11 09:56:12  giufor
#	Import libEOFPreInventory
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libGRIBPreInventory

VERSIONFILE = GRIBPreInventoryV.C

C++SOURCES = GRIBPreInventory.C \
	GRIBPreInventoryFactory.C \
	GRIBPreInventoryV.C

C++INCLUDE = GRIBPreInventory.h \
	GRIBPreInventoryFactory.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

# the grib api library has been replaced by libeccodes publishing the same APIs in another package
# the following definition can be overridden in the project MK file if needed
# please do not modify this makefile for this purpose
GRIBAPILIB?=-lgrib_api

DSOSPECIFICDEPS = \
	$(GRIBAPILIB) \
	-ljasper \



