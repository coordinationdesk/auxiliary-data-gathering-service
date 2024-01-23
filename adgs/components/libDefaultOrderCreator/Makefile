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
#	$Prod: A.C.S. Default Order Creator library $
#
#	$Author$
#	$Id$
#
#	$Log$
#	Revision 5.1  2014/05/16 10:09:50  marpas
#	helpspecific target fixed
#	
#	Revision 5.0  2013/06/19 18:07:26  marpas
#	adopting libException 5.x standards
#	types fixed
#	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
#	
#	Revision 2.7  2013/03/08 17:11:42  marpas
#	libDbBasic not longer used
#	
#	Revision 2.6  2012/09/18 09:24:16  marpas
#	NoMergeUsrDistribOrderCreator class added and NOMRG_USRDISTRIB algo implemented
#	
#	Revision 2.5  2011/09/22 13:52:10  marpas
#	test Makefile fixed but empty
#	
#	Revision 2.4  2009/03/06 16:40:28  ivafam
#	GOCELTA aligned to GOCE
#	
#	Revision 2.3  2008/10/07 12:43:47  crivig
#	porting to postgres
#	
#	Revision 2.2  2007/07/20 13:22:05  giucas
#	$PROJECT is used to define GOCE ... else classes will be misaligned
#	
#	Revision 2.1  2006/08/23 14:51:05  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:24:17  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.3  2005/11/28 20:18:02  marpas
#	NRTExternalOrderCreator class added - supports Envisat for EXTERNAL algorithm
#	
#	Revision 1.2  2005/10/24 17:46:08  marpas
#	dbexpress compatibilty
#	
#	Revision 1.1.1.1  2005/10/06 15:28:43  marfav
#	Import libDefaultOrderCreator
#	
#	
#
#
include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)


  LIBRARYNAME =	libDefaultOrderCreator

  VERSIONFILE =	dorcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		GroupedCollectionOrderCreator.C \
        NoMergeUsrDistribOrderCreator.C \
        	NoMergeRegXOrderCreator.C \
		MergeOrderCreator.C \
		NRTCreatorFactory.C \
		NRTLevel1OrderCreator.C \
		NRTExternalOrderCreator.C \
		NoMergeOrderCreator.C \
		OfflineAnx2AnxOrderCreator.C \
		OfflineCreatorFactory.C \
		RPROCreatorFactory.C \
		RPROExtOrderCreator.C \
		dorcV.C 


   C++INCLUDE = \
		GroupedCollectionOrderCreator.h \
        NoMergeUsrDistribOrderCreator.h \
		NoMergeRegXOrderCreator.h \
		MergeOrderCreator.h \
		NRTCreatorFactory.h \
		NRTLevel1OrderCreator.h \
		NRTExternalOrderCreator.h \
		NoMergeOrderCreator.h \
		OfflineAnx2AnxOrderCreator.h \
		OfflineCreatorFactory.h \
		RPROCreatorFactory.h \
		RPROExtOrderCreator.h 


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


# GOCE Specific libraries
ifeq ($(PROJECT), Goce)

PROJECTFLAG     = -DGOCE

endif

# GOCELTA Specific libraries
ifeq ($(PROJECT), GoceLTA)
#dbexpress compatibility mode (for Informix w/ SQLString)
CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)
PROJECTFLAG     = -DGOCE

endif

  USERFLAG = $(PROJECTFLAG)

TESTCOMPILERULE=
CLEANDEPENDTEST=
DEPENDTESTRULE=

DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk
