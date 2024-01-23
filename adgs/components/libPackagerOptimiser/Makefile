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
#	$Prod: A.C.S. libPackagerOptimiser Makefile $
#	
#       $Id$
#
#       $Author$
#
#       $Log$
#       Revision 2.3  2011/03/03 12:30:08  marpas
#       ihash_map changed into map
#
#       Revision 2.2  2008/01/30 17:45:55  enrcar
#       EC:: Updated to handle new factory PackagerOptimiser_OneItemPlusRpt
#
#


include ../../common/commondefs.mk


  LIBRARYNAME =	libPackagerOptimiser

  VERSIONFILE = poV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
                poV.C \
                ItemDesc.C \
                ItemPartition.C \
                PackagerOptimiser.C \
                PackagerOptimiser_DateSize.C \
                PackagerOptimiser_OneItem.C \
                PackagerOptimiser_OneItemPlusRpt.C \
                PackagerOptimiserFactory.C \
                PackagerOptimiserSingleton.C \
				dbDistributionItemExtended.C \
				dbDistributionItemExtendedQuery.C \
				dbPackagerOptimiserFactory.C


   C++INCLUDE = \
                PackagerOptimiserSingleton.h \
                PackagerOptimiserFactory.h \
                PackagerOptimiser_OneItemPlusRpt.h \
                PackagerOptimiser_OneItem.h \
                PackagerOptimiser_DateSize.h \
                PackagerOptimiser.h \
                ItemPartition.h \
                ItemDesc.h

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

