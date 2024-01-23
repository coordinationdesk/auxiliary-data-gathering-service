#
#  Copyright 1995-2022, Exprivia SpA.
#  Via Della Bufalotta, 378 - 00139 Roma - Italy
#  http://www.exprivia.com
#
#  All Rights Reserved.
#
#  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
#  the contents of this file may not be disclosed to third parties, copied or
#  duplicated in any form, in whole or in part, without the prior written
#  permission of Exprivia SpA.
#
#  $Prod: LTAQuotaService $
#
#

include ../../common/commondefs.mk

  USERFLAG = $(PROJECTFLAG)

  LIBRARYNAME = LTAQuotaService

  VERSIONFILE = ltaqsV.C


  C++SOURCES = LTAQuotaServiceConfigurer.C \
               BaseService.C \
               CancelOrderService.C \
               CheckBatchQuotaService.C \
               CheckOrderQuotaService.C \
               LTAQuotaService.C \
               CheckDownloadQuotaVolumeService.C \
               CheckParallelOrderQuotaService.C \
               UpdateOrderRegistryService.C \
               GetDownloadTokenService.C \
               UseDownloadTokenService.C \
               ReleaseDownloadTokenService.C \
               NotifyDownloadStatusService.C \
               ltaqsV.C

#prevent remotion of objects, target and versioning
      VERSIONRULE =
      TARGETFILES = LTAQuotaService
      CLEANTESTRULE=

SPECIFICLIBS= 


$(TARGETFILES) : $(OBJECTS)
	$(CXX) $(LDOPTS) $(CXXFLAGS) $(LIBRARYPATH) -o $@  $(OBJECTS)  $(DEPLIBS) $(PRJLIBS) $(SPECIFICLIBS)


COV_PATHS := $(shell pwd)/test
COV_BASE_DIR := $(shell pwd)

cleanspecific:

helpspecific:

include ../../common/commonrules.mk
