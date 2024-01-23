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
#	$Prod: A.C.S. Ftp Curl Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#
#

include ../../common/commondefs.mk


  LIBRARYNAME =	libFtpCurl

  VERSIONFILE = lfcV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
    BandPriority.C \
	CircularBuffer.C \
	CirculationCurl.C \
	CirculationStatistics.C \
	Curl.C \
	CurlConfigurer.C \
	CurlFile.C \
	CurlHeaderMaker.C \
	CurlStatistics.C \
	CurlTransaction.C \
	FilePermissions.C \
	FtpCurl.C \
	FtpListItem.C \
	FtpListReader.C \
	FtpPerformance.C \
	FtpProgressData.C \
	HttpProgressData.C \
	lfcV.C \
	RemoteDirVisitor.C \
	RemoteSite.C \
	RemoteSiteFactory.C \
	RemoteSiteUtility.C \
	StreamCurl.C \
	TemporaryFileService.C \
	TempStrategy.C \
	CurlErrorBehaviourSingleton.C \


C++INCLUDE = \
	AbstrBytesStream.h \
	BytesInputStream.h \
	BytesOutputStream.h \
	CircularBuffer.h \
	CirculationCurl.h \
	CirculationStatistics.h \
	CurlConfigurer.h \
	CurlFile.h \
	Curl.h \
	CurlHeaderMaker.h \
	CurlStatistics.h \
	CurlStream.h \
	CurlTransaction.h \
	FilePermissions.h \
	FtpCurlConstants.h \
	FtpCurlExceptions.h \
	FtpCurl.h \
	FtpListItem.h \
	FtpListReader.h \
	FtpPerformance.h \
	FtpProgressData.h \
	HttpProgressData.h \
	BandPriority.h \
	PipedBytesStream.h \
	RemoteDirVisitor.h \
	RemoteSiteFactory.h \
	RemoteSite.h \
	RemoteSiteUtility.h \
	StreamCurl.h \
	TemporaryFileService.h \
	TempStrategy.h \
	CurlErrorBehaviourSingleton.h \


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)
 
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS = -lcurl


include ../../common/commonrules.mk



