#
#	Copyright 1995-2021, Exprivia SpA - DADF
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.it
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Prod: A.C.S. libPackageExporter$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.10  2017/01/16 14:42:12  nicvac
#	New Makefile standard
#	
#	Revision 1.9  2016/12/19 18:32:44  nicvac
#	S3MPM-860: PackageExporter resources improvement.
#	
#	Revision 1.8  2016/12/13 16:39:57  nicvac
#	OPMAN-199: Reset intermediate status improved.
#	
#	Revision 1.7  2014/04/17 17:11:07  nicvac
#	S2PDGS-603: Scheduler log filtering.
#	
#	Revision 1.6  2013/09/30 12:57:11  marpas
#	useless includes and libraries removed
#	
#	Revision 1.5  2012/12/06 18:36:11  nicvac
#	Test fix
#	
#	Revision 1.4  2012/12/05 16:32:04  nicvac
#	lib list
#	
#	Revision 1.3  2012/11/20 18:07:17  nicvac
#	DataPackage and Distribution serialization.
#	
#	Revision 1.2  2012/11/12 17:44:24  nicvac
#	PackageProcessingManager integrated.
#	
#	Revision 1.1.1.1  2012/11/09 13:28:07  nicvac
#	Import PackageExporter
#	
#        
#

include ../../common/commondefs.mk

USERFLAG = $(PROJECTFLAG)

LIBRARYNAME = PackageExporter

VERSIONFILE = PackageExporterV.C

CUSTOM_INCLUDEPATH =

CUSTOM_LIBPATH =

CSOURCES =	

CINCLUDE =	

C++SOURCES = PackageExporter.C \
	DistributionListener.C \
	ResetStatus.C \
	ResourceStats.C \
	PackageExporterV.C

UIC_SOURCES= 

MOC_SOURCES = 

C++INCLUDE = 

SPECIFICLIBS = \
	-lboost_serialization 

EXES =  \
	PackageExporter

INST_EXECUTABLE = $(EXES)

TARGETFILES = $(EXES)

    
$(TARGETFILES):  $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


CLEANTESTRULE =
TESTCOMPILERULE=
DEPENDTESTRULE=
CLEANDEPENDTEST=

# DO NOT DELETE

include ../../common/commonrules.mk

