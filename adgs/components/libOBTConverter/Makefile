#
#	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. libOBTConverter $
#
#	$Log$
#	Revision 2.4  2007/07/25 17:47:25  nicvac
#	Support for Mos added.
#	
#	Revision 2.3  2006/10/19 07:59:51  marpas
#	header fixed
#	
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libOBTConverter

  VERSIONFILE = obtConvV.C

   C++SOURCES = OBTConverter.C \
		OBTConverterFactory.C \
		OBTConverterSingleton.C \
		GoceOBTConverter.C \
		ErsOBTConverter.C \
		ErsParamsOBTConverter.C \
		MosOBTConverter.C \
		EnvisatOBTConverter.C \
		obtConvV.C


   C++INCLUDE = OBTConverter.h \
		OBTConverterFactory.h \
		OBTConverterSingleton.h \
		GoceOBTConverter.h \
		ErsOBTConverter.h \
		ErsParamsOBTConverter.h \
		MosOBTConverter.h \
		EnvisatOBTConverter.h



COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	
#empty rule, do not remove, you can add specific help strings
helpspecific:
	
	
include ../../common/commonrules.mk
