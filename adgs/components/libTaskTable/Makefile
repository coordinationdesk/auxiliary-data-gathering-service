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
#	$Prod: A.C.S. ThinLayer TaskTable Library Makefile $
#	$Id$
#
#   $Log$
#   Revision 5.2  2014/06/10 15:49:47  marfav
#   Added support to DynProcParams and SensingTimeFlag
#
#   Revision 5.1  2014/05/16 09:39:08  marpas
#   fixing helpspecific target
#
#   Revision 5.0  2013/07/07 18:24:07  marpas
#   itroducing libException 5.x standards
#   coding best practices applied
#   qa warnings removed
#   compilation warnings partially removed
#
#   Revision 2.4  2010/08/03 14:28:06  nicvac
#   Task Table plugins management
#
#   Revision 2.3  2008/04/22 11:17:09  marfav
#   Design improved using the conditioned visitor pattern
#
#   Revision 2.2  2006/08/23 14:52:36  paoscu
#   Shared libraries dependencies added
#
#   Revision 2.1  2006/03/06 13:37:45  marfav
#   AtExit Sequences structure support added
#
#   Revision 2.0  2006/02/28 10:07:35  marpas
#   Exception 2.1 I/F adopted
#
#   Revision 1.7  2005/06/20 15:09:53  enrcar
#   INCLUDEPATH REMOVED
#
#   Revision 1.6  2004/09/14 17:05:22  marfav
#   Execution code moved to libTaskExecution
#
#   Revision 1.5  2004/05/25 16:24:41  marfav
#   Messages sent to Tracker issued using TrkFilterables
#
#   Revision 1.4  2003/08/20 09:54:51  envisat
#   Changed commondefs and commonrules includes paths from /raid0/cryosat to ../../
#
#   Revision 1.3  2003/02/05 13:41:43  fracar
#   handling T0 and T1 fields inside the task table
#
#   Revision 1.2  2003/01/31 14:01:51  marfav
#   handling different integration and test environments
#
#   Revision 1.1.1.1  2003/01/31 11:04:55  marfav
#   Import libTaskTable
#
#
#
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libTaskTable

  VERSIONFILE = tlttV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		TaskTable.C \
		DefaultRWDefinitions.C \
		DefaultTTWriter.C \
		DefaultTTReader.C \
		ResolverVisitor.C \
		TTSymbolTable.C \
		TaskTableFactoryChain.C \
		DefaultTaskTableFactory.C \
		DynProcParam.C \
		tlttV.C\


   C++INCLUDE = \
		TaskTable.h \
		DefaultRWDefinitions.h \
		DefaultTTWriter.h \
		DefaultTTReader.h \
		ResolverVisitor.h \
		TTSymbolTable.h \
		TaskTableFactoryChain.h \
		DefaultTaskTableFactory.h \
		DynProcParam.h \


 
DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:



include ../../common/commonrules.mk
