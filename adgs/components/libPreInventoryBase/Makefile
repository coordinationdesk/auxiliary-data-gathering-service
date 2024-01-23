#
#	Copyright 1995-2015, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libPreInventoryBase$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.9  2015/03/04 10:48:18  marpas
#	importing PDGSExtDataPreInventory class from libPDGSExtDataPreInventory
#	
#	Revision 1.8  2013/08/09 12:30:54  lucio.pulvirenti
#	SatelliteMissionAssociatorSingleton, SatelliteMissionAssociatorFactory, SatelliteMissionAssociator, StdSatelliteMissionAssociator, PlainSatelliteMissionAssociator classes added.
#	
#	Revision 1.7  2013/02/21 11:13:24  marpas
#	redesign to use connection pool and explicit connections
#	coding best practices added
#	test implementation completely rewritten
#	qa rules
#	
#	Revision 1.6  2012/12/14 15:53:46  chicas
#	added boost
#	
#	Revision 1.5  2012/12/06 14:51:16  lucio.pulvirenti
#	boost dependency removed.
#	
#	Revision 1.4  2008/10/23 10:50:06  nicvac
#	dso updated.
#	
#	Revision 1.3  2008/10/10 16:26:42  nicvac
#	DSO added.
#	
#	Revision 1.2  2008/10/08 13:55:29  nicvac
#	Attributes and interface updated.
#	
#	Revision 1.1.1.1  2008/09/30 14:35:50  nicvac
#	Import libPreInventoryBase
#	
#        
#

include ../../common/commondefs.mk

   LIBRARYNAME = libPreInventoryBase

   VERSIONFILE = PreInventoryBaseV.C

   CUSTOM_INCLUDEPATH =

   CUSTOM_LIBPATH =

   CSOURCES =	

   CINCLUDE =	

   C++SOURCES = \
            PreInventoryBase.C \
          	PreInventoryBaseV.C \
		    SatelliteMissionAssociator.C \
		    SatelliteMissionAssociatorSingleton.C \
		    SatelliteMissionAssociatorFactory.C \
		    StdSatelliteMissionAssociator.C \
		    PlainSatelliteMissionAssociator.C \
            PDGSExtDataPreInventory.C  \



   C++INCLUDE = \
            PreInventoryBase.h \
	        PreInventoryFactoryChain.h \
		    SatelliteMissionAssociator.h \
		    SatelliteMissionAssociatorSingleton.h \
		    SatelliteMissionAssociatorFactory.h \
		    StdSatelliteMissionAssociator.h \
		    PlainSatelliteMissionAssociator.h \
            PDGSExtDataPreInventory.h  \
            

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

DSOSPECIFICDEPS = -lboost_program_options
