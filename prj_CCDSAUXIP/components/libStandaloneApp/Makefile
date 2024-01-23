#
#	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Standalone Application Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.5  2016/03/16 16:35:05  marpas
#	qt management
#	
#	Revision 5.4  2014/05/20 12:16:13  marfav
#	Addedd support for IPF Standalone app
#	
#	Revision 5.3  2014/03/10 14:21:46  nicvac
#	Qt part: Porting to head.
#	
#	Revision 5.2  2013/10/22 16:57:37  marpas
#	dependencies fixed
#	
#	Revision 5.1  2013/10/21 14:23:37  marpas
#	dependencies fixed
#	
#	Revision 5.0  2013/06/20 08:41:56  marpas
#	adopting libException 5.x standards
#	coding best practices applied
#	compilation warnings
#	
#	Revision 1.7  2011/04/04 16:55:16  giucas
#	DSODEPENDLIBS updated for case ALLOWQTCOMPONENTS 0
#	
#	Revision 1.6  2010/11/11 13:18:21  giucas
#	Added check on ALLOWQTCOMPONENTS
#	
#	Revision 1.5  2007/02/26 15:48:28  marqua
#	uses pyStandaloneAppResourceManager and pyMessageQueue - therefore avoid declaring pyMWMessage in WRAPPERS
#	
#	Revision 1.4  2007/01/25 11:07:08  giucas
#	WRAPPERS for py added
#	
#	Revision 1.3  2006/12/01 15:58:54  clanas
#	DSODEPENDLIBS modified.
#	
#	Revision 1.2  2006/04/20 12:17:29  clanas
#	fixed version file name
#	
#	Revision 1.1.1.1  2006/04/20 12:15:46  clanas
#	imported source code
#	
#	
#
#	


include ../../common/commondefs.mk
  

  LIBRARYNAME =	libStandaloneApp

  VERSIONFILE = lsaV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = StandaloneApp.C \
		IPFLogConfiguration.C \
		IPFLogFmtFactory.C \
		IPFLogLevelFilter.C \
		IPFStandaloneApp.C \
                osfiIPFLogFmtFactory.C \
                osfiIPFStandaloneApp.C \
		lsaV.C

   C++INCLUDE = StandaloneApp.h \
		IPFLogConfiguration.h \
		IPFLogFmtFactory.h \
		IPFLogLevelFilter.h \
		IPFStandaloneApp.h \
                osfiIPFLogFmtFactory.h \
                osfiIPFStandaloneApp.h \


#WRAPPERS = pyStandaloneApp

ifeq ($(ALLOWQTCOMPONENTS), 1)
	C++SOURCES := StandaloneQtApp.C $(C++SOURCES)
	C++INCLUDE := StandaloneQtApp.h $(C++INCLUDE)
	MOC_SOURCES = StandaloneQtApp.h
    CXXAUXOPTS_QT = -Wall \
				 -Wnon-virtual-dtor \
				 -Wno-non-template-friend \
				 -Woverloaded-virtual \
				 -Wno-pmf-conversions \
				 -Wsign-promo 

				 
DSOSPECIFICDEPS = -lQtException


StandaloneQtApp.o: CXXAUXOPTS=$(CXXAUXOPTS_QT)
moc_StandaloneQtApp.o: CXXAUXOPTS=$(CXXAUXOPTS_QT)


else
DSOSPECIFICDEPS =

endif



include ../../common/commonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 



# DO NOT DELETE


