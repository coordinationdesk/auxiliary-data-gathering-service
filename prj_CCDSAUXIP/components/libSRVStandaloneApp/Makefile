#
#
#   Copyright 2019, Exprivia - DFDA-AS
#   Via Della Bufalotta, 378 - 00139 Roma - Italy
#   http://www.exprivia.it
#
#   All Rights Reserved.
#
#   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#   the contents of this file may not be disclosed to third parties, copied or
#   duplicated in any form, in whole or in part, without the prior written
#   permission of Exprivia SpA
#
#   $Prod: SRV Standalone App $
#
#


include ../../common/commondefs.mk
  

  LIBRARYNAME =	libSRVStandaloneApp

  VERSIONFILE = SRVStandaloneAppV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = SRVStandaloneApp.C \
 		SRVLogFmtFactory.C \
		SRVStandaloneAppV.C

   C++INCLUDE = SRVStandaloneApp.h \
		SRVLogFmtFactory.h \
	 

  DSOSPECIFICDEPS =  -lMQTTStatistics



include ../../common/commonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 



# DO NOT DELETE


