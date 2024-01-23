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
#	$Prod: A.C.S. Order Creator library $
#
#	$Author$
#	$Id$
#
#	$Log$
#	Revision 5.0  2013/06/19 13:54:01  marpas
#	satelliteid is now unsigned int - to be coherent with its NameIdAssociator
#	OrderCreator has 2 static methods to deal with null satellite.
#	libException 5.x standards adopted
#	qa rules
#	
#	Revision 2.6  2013/03/08 17:09:29  marpas
#	libDbBasic no longer used
#	
#	Revision 2.5  2013/02/13 18:24:26  marpas
#	new class introduced to execute order (application level)
#	coding best practices applied
#	
#	Revision 2.4  2012/03/07 13:09:24  marpas
#	refactoring
#	introducing XMLOrderParameters class and its unitary test
#	
#	Revision 2.3  2009/03/06 10:20:45  ivafam
#	Added GoceLTA specific define
#	
#	Revision 2.2  2007/07/19 12:42:52  marpas
#	GOCE work in progress
#	
#	Revision 2.1  2006/08/23 14:52:38  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:23:55  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.1.1.1  2005/10/06 15:09:05  marfav
#	Import libOrderCreator
#	
#	
#
#
include ../../common/commondefs.mk


  LIBRARYNAME =	libOrderCreator

  VERSIONFILE =	ogenV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		OrderExecutor.C \
		OrderCreator.C \
		OrderCreatorFactoryChain.C \
		OrderCreatorFactory.C \
		TemporaryOrder.C \
		OGDBSimulationSingleton.C \
        XMLOrderParameters.C \
		ogenV.C 


   C++INCLUDE = \
		OrderExecutor.h \
		OrderCreator.h \
		OGDBSimulationSingleton.h \
		OrderCreatorFactory.h \
		TemporaryOrder.h \
		OrderCreatorFactoryChain.h \
        XMLOrderParameters.h
  

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

# GOCE Specific libraries
ifeq ($(PROJECT), Goce)

PROJECTFLAG     = -DGOCE

endif

# GOCE-LTA Specific libraries
ifeq ($(PROJECT), GoceLTA)

PROJECTFLAG     = -DGOCE

endif


  USERFLAG = $(PROJECTFLAG)


DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk
