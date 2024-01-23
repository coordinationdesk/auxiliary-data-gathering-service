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
#	$Prod: A.C.S. Makefile for ThinLayer Daemon $
#
#	$Id$
#
#	$Author$
#

include ../../common/commondefs.mk





# User Flags
# SEPARATED_MCF_MESSAGES_LOG  -> stores mcf messages on a dedicated log file located in the standard application log path

USERFLAG = $(PROJECTFLAG) 


  VERSIONFILE =	tldV.C

     CSOURCES =	

     CINCLUDE =	

  LIBRARYNAME = TlDaemon

  CONF_FILES = tlDaemon.xml 

   C++SOURCES = \
		SignalGuard.C \
		DbOrderQueueMgr.C\
		FileOrderQueueMgr.C\
		InputDownloader.C \
		InputResourceMgr.C\
		InteractiveDbOrderQueueMgr.C\
		OrderExecutionMgr.C\
		OrderQueueMgr.C \
		QueueMgrFactory.C \
		tlDaemon.C\
		ExecutingOrder.C\
		TlStatusMgrSingleton.C\
		OrderReservedResources.C\
		OrderSlot.C\
		CacheManager.C\
		tldV.C\


		
   UIC_SOURCES= 
	
   MOC_SOURCES = 

   C++INCLUDE = 

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = \
		  TlDaemon \
		  

$(TARGETFILES) : $(OBJECTS) 
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)

CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:


include ../../common/commonrules.mk


# DO NOT DELETE

