#
#	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
#	$Author$ 
#
#	@(#) A.C.S. libFileInventory Makefile - %M% %I% %E%
#
#	$Log$
#	Revision 5.0  2013/06/20 07:48:54  marpas
#	implementing SENT-37 about messaging.
#	adoption of libException 5.x standards
#	rationalization and coding best practices applied.
#	uselsess SW removed
#	qa warnings removed
#	compilation warnings removed
#	
#	Revision 2.6  2013/05/24 18:28:40  nicvac
#	S2PDGS-308: Inventory notifications implemented.
#	
#	Revision 2.5  2010/09/22 09:22:42  marpas
#	slightly changed
#	
#

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME =	libFileInventory

  VERSIONFILE = FileInvV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = FileInvV.C \
   		Metadata.C \
   		FileInventory.C \
		FileInventoryVersion.C \
		PreInventoryObjList.C \
		GeoPointIterator.C \
		InventoryCovRuleChecker.C 
		
   
   C++INCLUDE = Metadata.h \
		FileInventory.h \
		FileInventoryVersion.h \
		PreInventoryObjList.h \
		GeoPointIterator.h \
		InventoryCovRuleChecker.h \
   				

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
include ../../common/commonrules.mk
	
