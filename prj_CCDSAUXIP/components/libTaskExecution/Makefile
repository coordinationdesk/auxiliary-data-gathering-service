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
#	$Prod: A.C.S. ThinLayer TaskExecution Library Makefile $
#	$Id$
#
#   $Log$
#   Revision 2.6  2017/09/22 09:29:33  marfav
#   Dependencies added
#
#   Revision 2.5  2017/09/20 13:12:36  davide.tiriticco
#   Refactoring parameter class
#
#   Revision 2.4  2017/09/19 09:58:58  davide.tiriticco
#   Added factory classes
#
#   Revision 2.3  2014/05/16 09:44:01  marpas
#   fixing helpspecific target
#
#   Revision 2.2  2008/04/22 11:13:21  marfav
#   Executors implemented using the conditioned visitor patter
#
#

include ../../common/commondefs.mk


				     
  LIBRARYNAME =	libTaskExecution

  VERSIONFILE = ttexecV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
   		TaskTableElementExecutor.C\
		ExecutorBase.C\
		ExecutorVisitor.C\
		DefaultExecutorVisitor.C\
		TaskTableExecutorFactoryChain.C\
		ExecutorVisitorFactoryChain.C\
		tlTaskLauncher.C\
		tlLogFile.C\
		ttexecV.C\


   C++INCLUDE = \
		TaskTableElementExecutor.h\
		ExecutorBase.h\
		ExecutorVisitor.h\
		DefaultExecutorVisitor.h\
		TaskTableExecutorFactoryChain.h \
		ExecutorVisitorFactoryChain.h \
		tlTaskLauncher.h\
		tlLogFile.h


#		DefaultTaskTableExecutorFactory.C\
		DefaultExecutorVisitorFactory.C\
		DefaultTaskTableExecutorFactory.h \
		DefaultExecutorVisitorFactory.h\

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared

DSOSPECIFICDEPS =


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

 
include ../../common/commonrules.mk
