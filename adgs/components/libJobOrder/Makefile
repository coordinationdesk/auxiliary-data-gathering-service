#
#	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
#	Authors: 
#
#	$Prod: A.C.S. ThinLayer JobOrder Library Makefile $
#	$Id$
#
#	$Log$
#	Revision 2.4  2014/05/16 09:41:34  marpas
#	fixing helpspecific target
#	
#	Revision 2.3  2012/10/17 10:25:19  chicas
#	Removed DefaultJobOrder
#	
#	Revision 2.2  2008/04/22 10:57:10  marfav
#	Aligned to new visitable task table elements
#	
#	Revision 2.1  2006/08/23 14:50:59  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:11:29  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.8  2005/09/07 11:27:15  nicvac
#	INCLUDEPATH definition removed.
#	
#	Revision 1.7  2005/08/31 08:00:16  clanas
#	Removed acs and xercex include
#	
#	Revision 1.6  2005/08/03 10:25:45  nicvac
#	Obsolete reference to ACE removed.
#	
#	Revision 1.5  2003/08/20 09:55:18  envisat
#	Changed commondefs and commonrules includes paths from /raid0/cryosat to ../../
#	
#	Revision 1.4  2003/03/26 15:23:28  marfav
#	Removed CryosatJobOrder embedded support
#	
#	Revision 1.3  2003/03/24 14:49:28  marfav
#	Added JobOrder Factory
#	
#	Revision 1.2  2003/02/05 13:43:51  fracar
#	fixed specific cleanup
#	
#	Revision 1.1.1.1  2003/01/31 15:00:48  marfav
#	Import libJobOrder
#	
#
#
#
include ../../common/commondefs.mk


  LIBRARYNAME =	libJobOrder

  VERSIONFILE =	tljoV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		JobOrder.C \
		JobOrderFactoryChain.C \
		tljoV.C \

   C++INCLUDE = \
		JobOrder.h \
		JobOrderFactoryChain.h \

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


#DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSODEPENDLIBS =  

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:


include ../../common/commonrules.mk
