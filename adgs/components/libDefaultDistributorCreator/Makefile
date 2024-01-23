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
#	Authors: 
#
#	$Prod:  $
#
#	$Author$
#	$Id$
#
#	$Log$
#	Revision 2.3  2015/05/15 16:31:05  nicvac
#	APF-264
#	
#	Revision 2.2  2014/05/16 10:13:36  marpas
#	helpspecific target fixed
#	
#	Revision 2.1  2013/03/08 17:18:19  marpas
#	libDbBasic no longer used
#	
#	Revision 2.0  2013/01/21 18:02:22  marpas
#	introducing dbConnectionPool use
#	coding best practices
#	filterable management improved
#	qa rules
#	
#	Revision 1.8  2012/05/18 15:27:09  marpas
#	version file MUST not be installed
#	
#	Revision 1.7  2012/02/29 15:20:41  chicas
#	Added
#	
#	Revision 1.6  2009/06/05 08:53:41  crivig
#	added new cvrg algos
#	
#	Revision 1.5  2008/10/07 10:35:29  crivig
#	porting to postgres8
#	
#	Revision 1.4  2008/02/01 15:10:21  crivig
#	qualifyfile algo added
#	
#	Revision 1.3  2007/04/02 14:46:39  crivig
#	*** empty log message ***
#	
#	Revision 1.2  2007/03/16 16:27:15  crivig
#	coding + testing
#	
#	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
#	Imported new source
#	
#	
#
#
include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)


  LIBRARYNAME =	libDefaultDistributorCreator

  VERSIONFILE =	ddrcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		AlwaysDistributorCreator.C \
		AlwaysDistributorFactory.C \
		QualifiedDistributorCreator.C \
		QualifiedDistributorFactory.C \
		PreselectionDistributorCreator.C \
		PreselectionDistributorFactory.C \
		RedistributionDistributorCreator.C \
		RedistributionDistributorFactory.C \
		QualifyReportDistributorCreator.C \
		QualifyReportDistributorFactory.C \
		QCCReportDistributorCreator.C \
		QCCReportDistributorFactory.C \
		QualifiedValiditystopDistributorCreator.C \
		QualifiedValiditystopDistributorFactory.C \
		QualifiedFileclassOriginatorDistributorCreator.C \
		QualifiedFileclassOriginatorDistributorFactory.C \
		TotalCvrgDistributorFactory.C \
		CvrgDistributorCreator.C \
		PartialCvrgDistributorFactory.C \
		DefaultDistributorFactories.C \
		ddrcV.C 


   C++INCLUDE = \
		AlwaysDistributorCreator.h \
		AlwaysDistributorFactory.h \
		QualifiedDistributorCreator.h \
		QualifiedDistributorFactory.h \
		PreselectionDistributorCreator.h \
		PreselectionDistributorFactory.h \
		RedistributionDistributorCreator.h \
		RedistributionDistributorFactory.h \
		QualifyReportDistributorCreator.h \
		QualifyReportDistributorFactory.h \
		QCCReportDistributorCreator.h \
		QCCReportDistributorFactory.h \
		QualifiedValiditystopDistributorCreator.h \
		QualifiedValiditystopDistributorFactory.h \
		QualifiedFileclassOriginatorDistributorCreator.h \
		QualifiedFileclassOriginatorDistributorFactory.h \
		TotalCvrgDistributorFactory.h \
		CvrgDistributorCreator.h \
		PartialCvrgDistributorFactory.h \
		DefaultDistributorFactories.h 

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:



include ../../common/commonrules.mk
