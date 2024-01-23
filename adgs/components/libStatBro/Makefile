#
#	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. StatBro Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.0  2013/07/02 11:22:11  marpas
#	adopting libException 5.x standards
#	coding best practices
#	qa warnings
#	
#	Revision 2.6  2013/03/29 19:52:21  marpas
#	removing html related functions
#	
#	Revision 2.5  2013/03/26 17:59:05  marpas
#	heavy refactoring, breaking comaptibility with Qt applications and StatisticalReportGenerator in favour f StatBroEngine
#	
#	Revision 2.4  2013/03/08 17:19:54  marpas
#	libDbBasic no longer used
#	
#	Revision 2.3  2008/11/14 14:39:23  marpas
#	moved here from application StatisticalReportGenerator
#	
#	

include ../../common/commondefs.mk


  LIBRARYNAME =	libStatBro

  VERSIONFILE = sbV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
   					sbV.C \
					ParamParser.C \
					ShowResult.C \
					XMLResult.C \
					ASCIIResult.C \
					Query.C \
					StatisticalReportGeneratorQuery.C \

					

   C++INCLUDE = \
					ParamParser.h \
					ShowResult.h \
					XMLResult.h \
					ASCIIResult.h \
					Query.h \
					StatisticalReportGeneratorQuery.h \


   MOC_SOURCES = 


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




DSOSPECIFICDEPS =

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help commands
helpspecific:

include ../../common/commonrules.mk
