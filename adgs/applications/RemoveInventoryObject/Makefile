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
#	$Author$
#
#	$Prod: $
#	$Id$
#
#   $Log$
#   Revision 1.17  2011/09/22 13:32:47  marpas
#   S1PDGS-1904 closure
#
#   Revision 1.16  2009/02/19 12:36:03  marpas
#   eless libraries removed
#
#   Revision 1.15  2009/01/21 17:12:31  daniela.bencivenni
#   Removed libQueryPref
#
#   Revision 1.14  2008/12/04 18:18:31  ivafam
#   libResultManager removed
#
#   Revision 1.13  2006/09/11 15:00:32  giapas
#   ADM specific section added.
#
#   Revision 1.12  2006/07/26 16:50:31  paoscu
#   -lStringUtils added
#   -lQtException removed
#
#   Revision 1.11  2006/05/09 14:25:31  tergem
#   Library order fixed.
#
#   Revision 1.10  2005/12/16 13:27:56  giapas
#   Added Gocecmf project.
#
#   Revision 1.9  2005/11/24 18:21:57  giapas
#   Suppressed Explorer CFI lib.
#   Project "Envisat" removed.
#
#   Revision 1.8  2005/11/14 18:22:16  giapas
#   Section for CompactStation added.
#
#   Revision 1.7  2005/11/03 17:03:27  paoscu
#   -lACS_SSL and -lCRC added.
#
#   Revision 1.6  2005/10/06 19:56:06  marpas
#   libSignal added
#
#   Revision 1.5  2005/08/02 12:12:31  marpas
#   useless libraries removed
#
#   Revision 1.4  2005/07/14 11:10:11  ivafam
#   Added project GoceLTA
#
#   Revision 1.3  2005/06/15 13:35:26  integrator
#   Added libStorageManager
#
#   Revision 1.2  2005/02/24 15:55:06  ivafam
#   Bug Fixed
#
#   Revision 1.1.1.1  2005/02/23 12:45:49  ivafam
#   Import RemoveInventoryObject 
#


include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = RemoveInventoryObject

  VERSIONFILE = riV.C

   C++SOURCES = \
   				riV.C \
				RemoveInventoryObject.C \
			
EXES =  \
	RemoveInventoryObject \
	

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      ALLRULE = alltest
      TARGETFILES=$(EXES)
      CLEANTESTRULE=


$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS)

cleanspecific:

helpspecific:

include ../../common/commonrules.mk
