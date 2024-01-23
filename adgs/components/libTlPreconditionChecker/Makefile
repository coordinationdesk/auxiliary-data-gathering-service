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
#	$Author$
#	$Prod: A.C.S. ThinLayer PreconditionChecker Library Makefile $
#
#	$Id$
#
#	$Log$
#	Revision 1.5  2014/05/16 09:45:50  marpas
#	fixing helpspecific target
#	
#	Revision 1.4  2008/08/04 10:16:22  marfav
#	Library split in definition and implementation
#	in order to support plugins management
#	
#	Revision 1.3  2008/04/29 12:45:40  marfav
#	Added support for PreconditionCheckFactoryChain
#	
#	Revision 1.2  2008/04/24 15:32:08  marfav
#	*** empty log message ***
#	
#	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
#	Importing libTlPreconditionChecker
#	
#	

include ../../common/commondefs.mk


  LIBRARYNAME =	libTlPreconditionChecker

  VERSIONFILE =	tlpccV.C

     CSOURCES =	

     CINCLUDE =	


   C++SOURCES = \
		AlternativeCheckerFactoryChain.C \
		PreconditionCheckerFactoryChain.C \
		Alternative_PCC.C \
		PCC_Base.C \
		ThinLayer_PCC.C \
		tlpccV.C \




   C++INCLUDE = \
		AlternativeCheckerFactoryChain.h \
		PreconditionCheckerFactoryChain.h \
		Alternative_PCC.h \
		ThinLayer_PCC.h \
		PCC_Base.h \

DSOSPECIFICDEPS =

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk
