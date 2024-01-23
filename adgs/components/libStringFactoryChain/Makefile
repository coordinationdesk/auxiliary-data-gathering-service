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
#	Revision 5.2  2016/11/21 16:05:34  marfav
#	S3MPM-847
#	Fixed library dependancies
#	
#	Revision 5.1  2014/05/16 09:32:46  marpas
#	fixing helpspecific target
#	
#	Revision 5.0  2013/07/07 17:35:32  marpas
#	adopting libException 5.x standards
#	coding best practices applied
#	qa warnings removed
#	compilation warnings removed
#	
#	Revision 1.3  2006/08/23 14:51:53  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.2  2005/06/20 15:29:54  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.1.1.1  2004/04/07 13:29:09  marfav
#	Import libStringFactoryChain
#	
#
#
include ../../common/commondefs.mk


  LIBRARYNAME =	libStringFactoryChain

  VERSIONFILE =	sfcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		TemplateKey.C \
		StringKey.C \
		TemplateExtendedEQ.C \
		StringFactoryChain.C \
		sfcV.C \


   C++INCLUDE = \
		TemplateKey.h \
		StringKey.h \
		TemplateExtendedEQ.h \
		StringFactoryChain.h\


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

include ../../common/commonrules.mk
