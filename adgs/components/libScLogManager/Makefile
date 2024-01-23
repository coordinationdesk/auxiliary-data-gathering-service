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
#	$Prod: A.C.S. Scheduler Log Manager Library Makefile $
#	$Id$
#
#	$Log$
#	Revision 5.0  2013/07/07 18:27:50  marpas
#	adopting libException 5.x standards
#	coding best practices applied
#	
#	Revision 1.4  2006/08/23 14:51:40  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.3  2004/09/23 15:17:19  marfav
#	New object hierarchy structure
#	
#	Revision 1.2  2004/09/15 09:14:22  marfav
#	TrkFilterable added to library
#	
#	Revision 1.1.1.1  2004/09/14 16:57:30  marfav
#	Import libScLogManager
#	
#

include ../../common/commondefs.mk


  LIBRARYNAME =	libScLogManager

  VERSIONFILE =	sclmV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		ScLogManager.C \
		ScFilterable.C \
		ScOutFilterable.C \
		ScErrFilterable.C \
		TrkFilterable.C \
		TrkHandler.C \
		ScLogHandlerFactoryChain.C \
		ScLogHandlerGroup.C \
		ScLogHandlerBase.C \
		sclmV.C


   C++INCLUDE = \
		ScLogManager.h \
		ScFilterable.h \
		ScErrFilterable.h \
		ScOutFilterable.h \
		TrkFilterable.h \
		TrkHandler.h \
		ScLogHandlerFactoryChain.h \
		ScLogHandlerBase.h \
		ScLogHandlerGroup.h 


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS =



include ../../common/commonrules.mk




