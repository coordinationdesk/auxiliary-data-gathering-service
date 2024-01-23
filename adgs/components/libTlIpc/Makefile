#
#	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. ThinLayer IPC Library Makefile $
#
#	$Id$
#
#  $Log$
#  Revision 2.4  2012/12/12 10:18:17  marpas
#  DSO dependencies minimized
#
#  Revision 2.3  2012/12/10 13:32:21  clanas
#  fixed DSO dependencies
#
#  Revision 2.2  2007/02/23 09:32:41  marant
#  Base classes moved into libIpcBase
#
#  Revision 2.1  2006/08/23 14:52:01  paoscu
#  Shared libraries dependencies added
#
#  Revision 2.0  2006/02/28 10:10:03  marpas
#  Exception 2.1 I/F adopted
#
#  Revision 1.4  2005/09/07 11:27:16  nicvac
#  INCLUDEPATH definition removed.
#
#  Revision 1.3  2005/08/31 07:48:53  clanas
#  Removed xercex include
#
#  Revision 1.2  2005/08/03 10:25:50  nicvac
#  Obsolete reference to ACE removed.
#
#  Revision 1.1.1.1  2004/04/07 13:33:14  marfav
#  Import libTlIpc
#
#


include ../../common/commondefs.mk


  LIBRARYNAME =	libTlIpc

  VERSIONFILE =	tlipcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
				TlMessage.C\
				TlIpcServer.C \
				TlIpcClient.C \
				ScIpcServer.C \
				ScIpcClient.C \
				tlipcV.C \

   C++INCLUDE = \
				TlMessage.h\
				TlIpcServer.h \
				TlIpcClient.h \
				ScIpcServer.h \
				ScIpcClient.h \
				

#this library has no test: override the rule
CLEANTESTRULE =

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =






#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

