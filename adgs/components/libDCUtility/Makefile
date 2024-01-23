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
#	$Prod:  $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.6  2017/10/23 13:57:01  marpas
#	test and coverage improved
#	
#	Revision 5.5  2017/10/23 09:59:01  marpas
#	getting rid of EntityBean and EntityManager template class
#	smartpersistentdef header files contained only macros to be used with the previous two
#	
#	Revision 5.4  2015/08/13 15:30:47  lucio.pulvirenti
#	S2PDGS-1249: MediaCirculationMngFunctions class added.
#	
#	Revision 5.3  2014/02/13 14:28:32  marpas
#	Crypto class removed
#	
#	Revision 5.2  2013/10/22 12:40:45  lucio.pulvirenti
#	AiDcConfigurationFunctions class added.
#	
#	Revision 5.1  2013/07/04 13:37:43  lucio.pulvirenti
#	TmpNameFileTransfer class added.
#	
#	Revision 5.0  2013/06/06 18:10:25  marpas
#	adopting libException 5.x standards
#	
#	Revision 1.16  2013/06/06 18:09:57  marpas
#	removed useless file
#	
#	Revision 1.15  2013/04/19 12:51:47  marpas
#	debug improved
#	
#	Revision 1.14  2013/04/03 15:37:54  marpas
#	useless class removed
#	
#	Revision 1.13  2013/03/14 15:28:46  marpas
#	dbConnectionPool use enforced
#	
#	Revision 1.12  2013/02/26 16:51:24  micmaz
#	added libDCApp library
#	
#	Revision 1.11  2013/02/18 18:39:17  micmaz
#	added db broadcast listener support
#	
#	Revision 1.10  2013/01/25 14:14:35  marpas
#	using pattern::Singleton from libException
#	
#	Revision 1.9  2013/01/10 17:57:54  micmaz
#	added phoenix callback
#	
#	Revision 1.8  2012/10/31 18:36:07  micmaz
#	added xml utility
#	
#	Revision 1.7  2012/10/23 15:08:39  micmaz
#	added findNativeQuery()
#	
#	Revision 1.6  2012/10/15 13:36:33  micmaz
#	Added a new configuration template class in order to simplifying the properties reading
#	
#	Revision 1.5  2012/06/18 12:35:54  micmaz
#	moved ConsumingRegister to DataConsumer library
#	
#	Revision 1.4  2012/04/02 14:42:14  micmaz
#	work in progress
#	
#	Revision 1.3  2012/03/27 13:30:39  micmaz
#	removed files.
#	
#	Revision 1.2  2012/03/15 16:50:13  micmaz
#	*** empty log message ***
#	
#	Revision 1.1  2012/02/28 17:21:11  micmaz
#	IN PROGRESS - issue S1PDGS-2945: The policy generator must return a specific code for each kind of error, and zero in all was ok.
#	http://jira.acsys.it/browse/S1PDGS-2945
#	
#		
#

include ../../common/commondefs.mk

LIBRARYNAME =	libDCUtility


VERSIONFILE = dcutilityV.C


C++SOURCES = \
	ConfigurationProperty.C \
	dcutilityV.C \
	XMLUtility.C \
	TmpNameFileTransfer.C \
	AiDcConfigurationFunctions.C \
	MediaCirculationMngFunctions.C \
	
C++INCLUDE =  \
	ConfigurationProperty.h  \
	DCConfigurer.h  \
	XMLUtility.h \
	TmpNameFileTransfer.h \
	AiDcConfigurationFunctions.h \
	MediaCirculationMngFunctions.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS =


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


 
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk



