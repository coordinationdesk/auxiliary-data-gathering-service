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
#	$Prod: A.C.S. Makefile for WS Config Merger $
#
#	$Id$
#
#	$Author$
#
#   $Log$
#   Revision 2.12  2014/05/16 10:06:27  marpas
#   helpspecific target fixed
#
#

include ../../common/commondefs.mk


USERFLAG = $(PROJECTFLAG) 

  VERSIONFILE =	wscmV.C

     CSOURCES =	

     CINCLUDE =	

  LIBRARYNAME = WSConfigMerger

   C++SOURCES = \
		WSConfigMerger.C \
		wscmV.C\


		
   UIC_SOURCES= 
	
   MOC_SOURCES = 

   C++INCLUDE = 




#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = \
		  WSConfigMerger \
		  
$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:

CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=


include ../../common/commonrules.mk


# DO NOT DELETE

