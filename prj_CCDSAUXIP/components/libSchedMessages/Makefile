#
#	Copyright 1995-2001, Advanced Computer Systems , Inc.
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
#
#	$Prod: A.C.S. Scheduler filterable messages wrapper Library Makefile $
#
#	$Id$
#
#   $Log$
#   Revision 1.4  2006/08/23 14:50:31  paoscu
#   Shared libraries dependencies added
#
#   Revision 1.3  2005/06/20 16:33:18  enrcar
#   INCLUDEPATH REMOVED
#
#   Revision 1.2  2003/08/20 09:53:20  envisat
#   Changed commondefs and commonrules includes paths from /raid0/cryosat to ../../
#
#   Revision 1.1.1.1  2003/07/16 09:44:32  marfav
#   Import libSchedMessages
#
#

include ../../common/commondefs.mk


  LIBRARYNAME =	libSchedMessages

  VERSIONFILE = scmsgV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		MsgSender.C \
		MsgSplitter.C \
		MsgFilterableWrp.C \
		scmsgV.C \

   C++INCLUDE = \
		MsgSender.h \
		MsgSplitter.h \
		MsgFilterableWrp.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

