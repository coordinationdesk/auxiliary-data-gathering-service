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
#	$Prod: A.C.S. Base IPC Library Makefile $
#
#	$Id$
#
#  	$Log$
#  	Revision 1.1.1.1  2007/02/19 11:05:17  marant
#  	Import libIpcBase
#  	
#
#


include ../../common/commondefs.mk


  LIBRARYNAME =	libIpcBase

  VERSIONFILE =	ipcbaseV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
				TcpIpIpcServer.C \
				TcpIpIpcClient.C \
				SigPipeManagerSingleton.C \
				SigPipeGuard.C \
				ipcbaseV.C \

   C++INCLUDE = \
				TcpIpIpcServer.h \
				TcpIpIpcClient.h \
				SigPipeManagerSingleton.h \
				SigPipeGuard.h \


#this library has no test: override the rule
CLEANTESTRULE =

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =






#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

