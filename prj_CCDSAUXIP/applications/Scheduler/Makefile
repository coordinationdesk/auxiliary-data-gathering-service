#
#	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Makefile for ThinLayer Scheduler $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.71  2016/04/06 11:41:24  marpas
#	unique Makefile
#	
#	Revision 1.70  2014/05/16 10:01:48  marpas
#	helpspecific target fixed
#	
#	Revision 1.69  2012/10/09 15:13:29  marfav
#	*** empty log message ***
#	
#	Revision 1.68  2012/02/28 14:30:28  marpas
#	no more warnings from Makefile
#	
#	Revision 1.67  2010/05/19 13:42:49  marpas
#	useless libraries removed
#	
#


include ../../common/commondefs.mk

#USERFLAGS
# -DRENAME_LOG_ON_EXIT  activate function to rename scheduler log to log.COMPLETED or log.ERROR on exiting

USERFLAG = $(PROJECTFLAG)

  VERSIONFILE = tlsV.C

     CSOURCES =	

     CINCLUDE =	

  LIBRARYNAME = Scheduler

   C++SOURCES = \
		scheduler.C\
		tlsV.C\


		
   UIC_SOURCES= 
	
   MOC_SOURCES = 

   C++INCLUDE = \


#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = \
		  Scheduler \
		  

OPTFLAG = -O0 -ggdb
OPTIMIZEFLAGS = -O0

$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:



# DO NOT DELETE

include ../../common/commonrules.mk

