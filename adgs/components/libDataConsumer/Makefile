#
#
#  Copyright 1995-2011, Advanced Computer Systems , Inc.
#  Via Della Bufalotta, 378 - 00139 Roma - Italy
#  http://www.acsys.it
#
#  All Rights Reserved.
#
#  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#  the contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of Advanced Computer Systems, Inc.
#
#  $Prod: A.C.S. Data Consumer Library $
#
#  $Id$
#
#  $Author$
#
#  $Log$
#  Revision 5.3  2014/06/25 17:29:46  marpas
#  Logger.C and Logger.h removed
#
#  Revision 5.2  2013/10/30 17:50:47  marpas
#  coding best practices applied
#  robustness improved
#  useless classes removed
#  compilation and qa warnings removed
#
#  Revision 5.1  2013/09/30 12:39:07  marpas
#  the following classes have been removed:
#  DataPackageItem
#  DataPackageRegister
#  DataReporter
#  QueryBuilder
#  SyncDataConsumer
#  dcSendMail
#  adopting libDbCryosat 4.x interfaces
#
#  Revision 5.0  2013/06/06 20:55:21  marpas
#  adopting libException 5.x standards
#
#  Revision 1.24  2012/06/18 12:35:54  micmaz
#  moved ConsumingRegister to DataConsumer library
#
#  Revision 1.23  2012/03/29 10:09:26  micmaz
#  work in progress
#
#  Revision 1.22  2012/02/28 17:19:51  micmaz
#  refactory Data circulator
#
#  Revision 1.21  2012/02/07 10:25:56  micmaz
#  removed warnings.
#
#  Revision 1.20  2011/10/28 15:24:52  micmaz
#  IN PROGRESS - issue S1PDGS-2274: PolicyGenerator Resource not found
#  http://jira.acsys.it/browse/S1PDGS-2274
#
#  Revision 1.19  2011/09/23 09:14:42  micmaz
#  removed unused headers.
#
#  Revision 1.18  2011/09/13 16:31:05  micmaz
#  IN PROGRESS - issue S1PDGS-1781: Problems with loading of the Configuration Parameters
#  http://jira.acsys.it/browse/S1PDGS-1781
#
#  Revision 1.17  2011/08/29 15:03:33  micmaz
#  remove DataSource
#
#  Revision 1.16  2011/07/15 17:09:41  micmaz
#  work in progress
#
#  Revision 1.15  2011/07/11 09:20:02  micmaz
#  added dependency to libThread
#
#  Revision 1.14  2011/04/11 12:24:53  micmaz
#  work in progress
#
#  Revision 1.13  2011/04/05 17:15:57  micmaz
#  *** empty log message ***
#
#  Revision 1.12  2011/03/01 17:31:11  micmaz
#  removed boost::thread for  added acs::Thread
#
#  Revision 1.11  2011/02/08 17:47:08  micmaz
#  removed comments.
#
#  Revision 1.10  2011/01/21 18:07:19  micmaz
#  data consumer rework.
#
#  Revision 1.9  2010/12/23 11:15:02  micmaz
#  first testable version.
#
#  Revision 1.8  2010/12/17 10:29:20  micmaz
#  Added MCF messages.
#
#  Revision 1.7  2010/12/09 16:22:14  micmaz
#  added Mail Warning.
#
#  Revision 1.6  2010/12/03 14:23:12  micmaz
#  The Registers now use the Stopper token in order to synch stopping / terminating events.
#
#  Revision 1.5  2010/11/30 16:42:25  micmaz
#  added mail warning.
#
#  Revision 1.4  2010/11/22 13:01:14  micmaz
#  added the product logger, correct some minor bugs.
#
#  Revision 1.3  2010/11/11 17:45:02  micmaz
#  corrected some issues : the consumers terminated with a coredump, the consumer hangs on when stopping with errored items, the data consumer item factory does not build correctly a DataConsumerItem.
#
#  Revision 1.2  2010/11/05 16:40:29  micmaz
#  The syncContainer must only be a synchronized channel. other responsibilites should be in charge to the Register.
#
#  Revision 1.1.1.1  2010/11/04 10:18:56  micmaz
#  create a new library
#
#
#
#

include ../../common/commondefs.mk


VERSIONFILE = DataConsumerV.C

LIBRARYNAME =	libDataConsumer

CSOURCES =	

CINCLUDE =	

C++SOURCES = \
	ConsumableFtpCurl.C \
	ConsumableItem.C \
	DataConsumerConfigurer.C \
	DataConsumerV.C 
				
C++INCLUDE = \
	ConsumableFtpCurl.h \
	ConsumableItem.h \
	ConsumingRegister.h \
	DataConsumerConfigurer.h  \
	GenericRegister.h \
	Indexed.h \
	RegistrableItem.h \
	SyncConsumer.h \
	SyncContainer.h \
	SyncPriorityConsumer.h \
	SyncPriorityContainer.h \
	SyncPriorityRegister.h \


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

     
DSOSPECIFICDEPS =

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:




include ../../common/commonrules.mk



