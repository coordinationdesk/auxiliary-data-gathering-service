#
#	Copyright 1995-2009, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Cryosat JobOrder Library Makefile $
#
#	$Id$
#
#	$Log$
#	Revision 1.3  2018/01/08 11:34:00  fabrizio.tringali
#	dso depends updated
#	
#	Revision 1.2  2009/09/11 16:37:02  marpas
#	fixed - compilation of .a and .so depend on project common files
#	
#	Revision 1.1.1.1  2009/03/02 14:40:14  chicas
#	Just imported
#	


#                       WARNING
# In case of plugins it is possibly harmful to link static libraries
# and a only those striclty necessary must be included
# The following tests is not-plugins libraries are compiled as shared or not
# And the value of DSODEPENDLIBS depends on this information.
# Note that this test must be performed BEFORE TARGETFILES is overwritten

 LIBRARYNAME =	libOcadaJobOrder

include ../../common/commondefs.mk

 DEFAULT_TARGETFILES := $(TARGETFILES)



ifeq "$(strip $(filter $(DSO_LIBRARY) , $(DEFAULT_TARGETFILES) ) )"  "$(DSO_LIBRARY)"
   DSOSPECIFICDEPS =

endif

  VERSIONFILE =	ocdjoV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		OcadaJobOrder.C \
		OcadaJobOrderFactory.C \
		ocdjoV.C


   C++INCLUDE = \
		OcadaJobOrder.h \
		OcadaJobOrderFactory.h \



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


#empty rule, do not remove, you can add specific remotion commands
helpspecific:


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 



include ../../common/commonrules.mk




