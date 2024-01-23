#
#	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. XMLResources Library $
#
#
include ../../common/commondefs.mk

  LIBRARYNAME =	libXMLResources

  VERSIONFILE = xmlResV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
	XMLIstream.C \
	XMLHandler.C \
	XMLResourceHandler.C \
	XMLExtractorHandlerBase.C \
	XMLExtractorHandler.C \
	XMLFilterHandler.C \
	BinSeekFileInputStream.C \
	LocalSeekFileInputSource.C \
	RedirectHandler.C \
	ResourceElement.C \
	ResourceElementAttribute.C \
	XMLOstream.C \
	XMLTransformer.C \
	XMLIstringstream.C \
	XMLDataConsumer.C \
	XMLProgDataConsumer.C \
	XMLValidator.C \
	XMLSubTreeXtractor.C \
	XMLProcessor.C \
	XmlDoc.C \
	UtilXml.C \
    XMLInitializer.C \
	xmlResV.C

   C++INCLUDE = \
	XMLIstream.h \
	XMLHandler.h \
	XMLOstream.h \
	XMLIstringstream.h \
	XMLTransformer.h \
	XMLDataConsumer.h \
	XMLProgDataConsumer.h \
	XMLExtractorHandlerBase.h \
	XMLValidator.h \
	XMLSubTreeXtractor.h \
	XMLProcessor.h \
	XmlDoc.h \
	UtilXml.h

	
DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS = -lxerces-c -lxml2


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk
