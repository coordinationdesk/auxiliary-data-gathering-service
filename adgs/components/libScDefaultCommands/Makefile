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
#	Authors: 
#
#	$Prod: A.C.S. Scheduler Default Commands Library Makefile $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.11  2006/08/25 16:28:26  paoscu
#	DSODEPENDLIBS fixed.
#	
#	Revision 1.10  2006/08/23 14:52:16  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.9  2006/01/19 13:18:47  giapas
#	Added target helpspecific.
#	Now doesn't install plugings in /usr/acs/plugins/SchedulerPlugins.
#	
#	Revision 1.8  2005/07/07 09:34:47  marfav
#	Creation of both static and dynamic libraries
#	
#	Revision 1.7  2004/09/14 15:29:22  marfav
#	Merging conflicts cleared
#	
#	Revision 1.6  2004/09/09 07:37:11  marfav
#	Target DSO path declaration uncommented
#	
#	Revision 1.5  2004/09/08 19:16:08  marpas
#	more comments
#	
#	Revision 1.4  2004/09/08 19:15:14  marpas
#	install target remove, relies on commonrules.mk one
#	
#	Revision 1.2  2004/08/25 15:38:58  marfav
#	Installing in SchedulerPlugins dir
#	
#	Revision 1.1.1.1  2004/04/07 13:31:48  marfav
#	Import libScDefaultCommands
#	
#


#                       WARNING
# In case of plugins it is possibly harmful to link static libraries
# and a only those striclty necessary must be included
# The following tests is not-plugins libraries are compiled as shared or not
# And the value of DSODEPENDLIBS depends on this information.
# Note that this test must be performed BEFORE TARGETFILES is overwritten

 LIBRARYNAME =	libScDefaultCommands

include ../../common/commondefs.mk

 DEFAULT_TARGETFILES := $(TARGETFILES)



DSOSPECIFICDEPS =

# Overwrite here TARGETFILES to make sure a shared object is prepared
# This could be removed once every library is compiled as shared object.

TARGETFILES=$(DSO_LIBRARY) 




  VERSIONFILE = scdcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		DefaultSuspendCmd.C \
		DefaultResumeCmd.C \
		DefaultAbortCmd.C \
		DefaultStopCmd.C \
		DefaultUserCmd.C \
		DefaultKillCmd.C \
		DefaultSchedCmdFactory.C \
		scdcV.C \


   C++INCLUDE = \
		DefaultSuspendCmd.h \
		DefaultResumeCmd.h \
		DefaultAbortCmd.h \
		DefaultUserCmd.h \
		DefaultStopCmd.h \
		DefaultKillCmd.h \
		DefaultSchedCmdFactory.h \





#empty rule, do not remove, you can add specific help strings
cleanspecific:


#empty rule, do not remove, you can add specific help strings
helpspecific:


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk



