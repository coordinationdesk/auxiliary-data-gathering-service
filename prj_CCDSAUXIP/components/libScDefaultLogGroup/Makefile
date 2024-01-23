#
#	Copyright 1995-2004, Advanced Computer Systems , Inc.
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
#	Revision 1.3  2006/08/23 14:51:16  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.2  2006/01/19 13:24:29  giapas
#	Added target helpspecific.
#	Now doesn't install plugings in /usr/acs/plugins/SchedulerPlugins.
#	
#	Revision 1.1.1.1  2004/09/14 16:36:52  marfav
#	Import libScDefaultLogGroup
#	
#

include ../../common/commondefs.mk


  LIBRARYNAME =	libScDefaultLogGroup

  VERSIONFILE =	scdlgV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		ScDefaultLogGroupFactory.C \
		ScDefaultLogHandlerGroup.C \
		scdlgV.C


   C++INCLUDE = \
		ScDefaultLogGroupFactory.h \
		ScDefaultLogHandlerGroup.h \

  DSOSPECIFICDEPS =



  DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared -L$(LIB_INTEGRATION_PATH)/lib

  TARGETFILES =  $(DSO_LIBRARY)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


#empty rule, do not remove, you can add specific remotion commands
helpspecific:


include ../../common/commonrules.mk
