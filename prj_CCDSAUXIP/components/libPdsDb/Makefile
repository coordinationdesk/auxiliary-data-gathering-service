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
#   $Prod: LTA PDS Db library $
#
#


include ../../common/commondefs.mk
  

  LIBRARYNAME =	libPdsDb

  VERSIONFILE = libpdsdbV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = libpdsdbV.C \
				FileType.C \
				FileTypeRecognition.C \
				LTAInventoryObject.C \
				LTAAipContent.C \
				LTAAip.C \
				LTAOrder.C \

   C++INCLUDE = FileType.h\
				FileTypeRecognition.h \
				LTAInventoryObject.h \
				LTAAipContent.h \
				LTAAip.h \
				LTAOrder.h \

  DSODEPENDLIBS =

		
COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk

