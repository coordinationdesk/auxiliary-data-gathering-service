#
#	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libThinLayerServices$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.7  2017/10/17 15:27:20  nicvac
#	ECICMF-93: Base and default TL Service Resources
#	
#	Revision 1.6  2017/10/17 11:32:53  nicvac
#	Refactored
#	
#	Revision 1.5  2017/10/13 16:20:20  nicvac
#	First component test
#	
#	Revision 1.4  2017/10/13 14:46:39  nicvac
#	Class integration
#	
#	Revision 1.3  2017/10/13 11:04:45  nicvac
#	Dependencies revised.
#	
#	Revision 1.2  2017/10/11 17:05:21  nicvac
#	ECICMF-93: Implementazione servizio Rest.
#	
#	Revision 1.1.1.1  2017/10/10 15:09:32  nicvac
#	Import libThinLayerServices
#	
#        
#

include ../../common/commondefs.mk

LIBRARYNAME = libThinLayerServices

VERSIONFILE = ThinLayerServicesV.C

CUSTOM_INCLUDEPATH = 

CUSTOM_LIBRARYPATH = 

CSOURCES =	

CINCLUDE =	

C++SOURCES = \
	ThinLayerServicesV.C \
	TLServiceResourceDefaultFactory.C \
	TLServiceResourceClientBase.C \
	TLServiceResourceSrvBase.C \
	TLServiceSlotClient.C \
	TLServiceSlotSrv.C \


C++INCLUDE = \
	ThinLayerServicesFactoryChain.h \
	TLServiceResourceDefaultFactory.h \
	TLServiceResourceClientBase.h \
	TLServiceResourceSrvBase.h \
	TLServiceSlotClient.h \
	TLServiceSlotSrv.h \

	
DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

DSOSPECIFICDEPS =

