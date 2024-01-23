#
#	Copyright 2019, Exprivia - DFDA-AS
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	$Author: $
#
#	$Prod: rest base interface $
#


include ../../common/commondefs.mk

  LIBRARYNAME =	libRestBase

  VERSIONFILE = RestBaseV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
        RestServerBase.C \
	RestServerConfigurer.C \
        RestBaseV.C

   C++INCLUDE = \
        RestServerBase.h \
	RestServerConfigurer.h \



COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


# DO NOT DELETE
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# adding a zero to avoid problems in case of emtpy GPPNUMERICALVERSION variable (old version of GITPortingTools)
GCCMORETHAN91 := $$( expr 0$(GPPNUMERICALVERSION) '>=' 9001000 )

#-lstdc++fs shall be removed when using gcc >= 9.1 
STDCFSINCLUDE := $$( if [ "$(GCCMORETHAN91)" == "0" ]; then echo -lstdc++fs; fi)


DSOSPECIFICDEPS =  -lpistache $(STDCFSINCLUDE)


include ../../common/commonrules.mk

