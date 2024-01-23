#
#	Copyright 1995-2005, Advanced Computer Systems , Inc.
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
#	$Prod: $
#	$Id$
#
#   $Log$
#   Revision 1.11  2017/03/20 09:20:05  clanas
#   modified to use new dependency mechanism
#
#   Revision 1.10  2013/05/12 14:37:10  marpas
#   no longer master/slave check
#
#   Revision 1.9  2013/02/05 18:17:24  marpas
#   listener re-added on t_temporarydistribution & t_completedorders
#   logical flow with delay rewritten in main loop
#
#   Revision 1.8  2012/03/08 10:52:55  marpas
#   task flow and messages revisited
#
#   Revision 1.7  2012/02/28 14:53:01  marpas
#   no more warnings from Makefile
#
#   Revision 1.6  2011/03/11 13:56:53  clanas
#   removed unneed lib
#
#   Revision 1.5  2009/02/19 12:01:29  marpas
#   debug improved and useless libraries removed
#
#   Revision 1.4  2009/01/21 16:45:22  daniela.bencivenni
#   removed libQueryPref
#
#   Revision 1.3  2008/12/04 18:18:30  ivafam
#   libResultManager removed
#
#   Revision 1.2  2007/05/28 07:22:40  ivafam
#   Added to Gocecmf
#
#   Revision 1.1.1.1  2007/03/15 15:01:03  crivig
#   Imported source
#

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = DistributorGenerator

  VERSIONFILE = dgV.C



   C++SOURCES = \
 		DistributorGenerator.C \
        DGListener.C \
		dgV.C

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = DistributorGenerator
      CLEANTESTRULE=

ORPHANLIBS = \
	-lDefaultDistributorCreator

$(TARGETFILES):  $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(ORPHANLIBS)

CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=


cleanspecific:

helpspecific:

include ../../common/commonrules.mk

