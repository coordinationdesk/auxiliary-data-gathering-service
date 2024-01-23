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
#	$Prod: A.C.S. Abstract Factory Library Makefile$
#	$Id$
#
#	$Log$
#	Revision 3.0  2016/11/21 16:02:25  marfav
#	Major release
#	
#	Revision 2.2  2014/05/16 09:31:54  marpas
#	fixing helpspecific target
#	
#	Revision 2.1  2006/08/23 14:52:28  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:08:02  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.4  2005/09/08 14:17:02  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.3  2005/08/31 07:44:00  clanas
#	Removed xercex include
#	
#	Revision 1.2  2005/08/03 10:25:44  nicvac
#	Obsolete reference to ACE removed.
#	
#	Revision 1.1.1.1  2004/04/07 13:28:30  marfav
#	Import libAbstractFactoryChain
#	
#
#
include ../../common/commondefs.mk


  LIBRARYNAME =	libAbstractFactoryChain

  VERSIONFILE =	afcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		AbstractFactoryChain.C \
		AbstractFactoryChainSingleton.C \
		afcV.C \


   C++INCLUDE = \
		AbstractFactoryChain.h \
		AbstractFactoryChainSingleton.h \






DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:



include ../../common/commonrules.mk


