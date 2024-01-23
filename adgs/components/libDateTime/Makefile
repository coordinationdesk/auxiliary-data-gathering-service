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
#	$Prod: A.C.S. DateTime Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.2  2014/04/01 10:55:02  marpas
#	starting heavy refactorying, implementing speed and accuracy
#	
#	Revision 5.1  2013/07/23 12:08:58  marpas
#	interface rationalization
#	performances improved
#	fixed timezone bug
#	test imaproved
#	
#	Revision 5.0  2013/07/01 15:28:51  marpas
#	adopting libException 5.x standards
#	coding best practices applied
#	qa warnings removed
#	compilation warnings removed
#	
#	Revision 2.8  2013/04/23 14:21:46  marpas
#	getting rid of libFeatures
#	
#	Revision 2.7  2012/12/02 20:04:10  marpas
#	qa rules
#	
#	Revision 2.6  2012/02/08 09:52:54  marpas
#	refactoring
#	
#	Revision 2.5  2008/10/08 09:21:37  marpas
#	include fixed
#	
#	Revision 2.4  2007/11/22 11:29:18  lucio.pulvirenti
#	-lFeatures added to libs.
#	
#	Revision 2.3  2007/02/21 15:46:22  marqua
#	added WRAPPERS
#	
#	Revision 2.2  2006/09/19 10:53:11  marpas
#	helpspecific target was missing
#	inclusion of commonrules should be last stmt of Makefile
#	
#	Revision 2.1  2006/08/23 14:50:55  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:40:49  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.5  2005/09/08 14:17:17  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.4  2004/08/03 13:48:24  marfav
#	Added TimeRange class to libDateTime
#	
#	Revision 1.3  2003/06/10 09:30:00  marpas
#	new class added
#	
#	Revision 1.2  2003/01/31 14:58:08  paoscu
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
#	Import libDateTime
#	
#	
#	

include ../../common/commondefs.mk


#must be an absolute path for coverage 
CUSTOM_INCLUDEPATH = -I$(shell pwd)/..
#must be an absolute path for coverage 
CUSTOM_LIBRARYPATH = -L$(shell pwd)/..
#must be an absolute path to avoid linking the installed one
CUSTOM_LDOPTS = -Wl,-rpath,$(shell pwd)/..

  LIBRARYNAME =	libDateTime

  VERSIONFILE = dtV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = DateTime.C \
   				TimeRange.C \
   				DateTimeManip.C \
				VariableDateTime.C \
   				dtV.C

   C++INCLUDE = DateTime.h \
				TimeRange.h \
   				VariableDateTime.h 

 

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 
DSOSPECIFICDEPS = -lboost_date_time

#empty rule, do not remove, you can add specific help strings
helpspecific:


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


include ../../common/commonrules.mk

# DO NOT DELETE


