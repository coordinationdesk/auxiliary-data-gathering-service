#
#	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libDataProvider

  VERSIONFILE = DataProviderV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		DataFormatter.C \
		DataFormatterFactoryChain.C \
		DataPackage.C \
		DataProvider.C \
		DataProviderV.C \
		S2GppMediaFormatter.C \
		DataFormatterFactory.C \
		DataProviderConfigurer.C \
		DownloadPackData.C \
		SingleNodeFtpFormatter.C

   C++INCLUDE = \
		DataFormatterFactoryChain.h \
		DataFormatter.h \
		DataPackage.h \
		DataProviderConfigurer.h \
		DataProvider.h \
		DownloadPackData.h \
		S2GppMediaFormatter.h \
		DataFormatterFactory.h \
		SingleNodeFtpFormatter.h

DSOSPECIFICDEPS = -lboost_serialization

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE
include ../../common/commonrules.mk

