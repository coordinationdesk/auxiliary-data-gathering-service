#
#	Copyright 1995-2011, Advanced Computer Systems , Inc.
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
#	$Author$ 
#
#	@(#) A.C.S. lib Safe Wrapper Base - %M% %I% %E%
#
#	$Log$
#	Revision 1.3  2011/09/08 07:37:01  aleber
#	class ProcessingContainerBase added
#	
#	Revision 1.2  2011/08/01 10:22:12  aleber
#	classes AcquisitionContainerBase and ProductContainerBase added
#	
#	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
#	First implementation
#	
	
#	

include ../../common/commondefs.mk

  LIBRARYNAME =	libSafeProductBase

  VERSIONFILE = l0SafeProdBaseV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = PlatformContainerBase.C \
   		QualityContainerBase.C \
		OrbitRefContainerBase.C \
		FrameSetContainerBase.C \
		AcquisitionContainerBase.C \
		ProcessingContainerBase.C \
		ProductContainerBase.C \
		ObjectContainerBase.C \
		ManifestBuilderBase.C \
		ManifestParametersListBuilder.C \
		l0SafeProdBaseV.C 

   C++INCLUDE = PlatformContainerBase.h \
   		QualityContainerBase.h \
		OrbitRefContainerBase.h \
		AcquisitionContainerBase.h \
		ProductContainerBase.h \
		ProcessingContainerBase.h \
		FrameSetContainerBase.h \
		ObjectContainerBase.h \
		ManifestBuilderBase.h \
		ManifestParametersListBuilder.h

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


include ../../common/commonrules.mk
DSOSPECIFICDEPS =

COV_PATHS := $(shell pwd)/test
COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
	
