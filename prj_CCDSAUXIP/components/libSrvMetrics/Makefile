#
#
#   Copyright 2021, Exprivia - DFDA-AS
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
#   $Prod: SRV Metrics library $
#
#


include ../../common/commondefs.mk
  

  LIBRARYNAME =	libSrvMetrics

  VERSIONFILE = lsrvmetV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = lsrvmetV.C \
                InventoryTotalMetrics.C \
                MetricsDbFunctions.C \
                MetricsHandlerFactory.C \
                BaseMetricsHandler.C \
                DailyMetricsHandler.C \
                MonthlyMetricsHandler.C \

   C++INCLUDE = InventoryTotalMetrics.h \
                MetricsDbFunctions.h \
                MetricsHandlerFactory.h \
                BaseMetricsHandler.h \
                DailyMetricsHandler.h \
                MonthlyMetricsHandler.h \

  DSODEPENDLIBS =

		
COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk

