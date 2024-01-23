#
#	Copyright 1995-2021, Exprivia SpA.
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA.
#
#	Authors: 
#
#	$Prod: A.C.S. file tools library $
#	$Id$
#
#   $Log$
#   Revision 5.5  2015/09/22 16:11:28  lucio.pulvirenti
#   EC:: LP:: Added support for computing GIT-style Digest (MD5/SHA1) for files and directories
#
#   Revision 5.4  2014/03/11 09:01:21  marfav
#   FSTree include fixed
#
#   Revision 5.3  2014/02/07 15:32:59  marfav
#   FSTree class added
#
#   Revision 5.2  2013/10/22 14:04:53  marpas
#   introducing thread safe Password class
#
#   Revision 5.1  2013/10/01 17:22:18  marpas
#   Networking class (Network) introduced
#
#   Revision 5.0  2013/06/20 16:43:53  marpas
#   adoption of libException 5.x in progress
#
#   Revision 2.16  2012/12/05 10:23:17  marpas
#   getting rid of FilenameUtils
#   testFilenameUtils rewritten using File.h
#   test/Makefile improved and rationalized
#
#   Revision 2.15  2012/11/29 19:31:29  marpas
#   implementing qa rules
#   optimizing code and coding best practices
#   coverage framework initiated
#
#   Revision 2.14  2012/10/05 16:14:01  lucio.pulvirenti
#   DiskEventNotifier class added.
#
#   Revision 2.13  2012/10/04 13:30:38  lucio.pulvirenti
#   Notify class added.
#
#   Revision 2.12  2011/06/07 14:18:56  marpas
#   minor changes to compile with the new compiler and new env constraints
#
#   Revision 2.11  2010/11/25 20:26:31  marpas
#   ache work in progress
#
#   Revision 2.10  2010/11/11 16:18:54  enrcar
#   Cpuid (Intel architecture) feature handled
#
#   Revision 2.9  2010/10/25 16:57:47  micmaz
#   Added a filename utility.
#
#   Revision 2.8  2009/03/13 15:26:45  marpas
#   some rule violations fixed
#
#   Revision 2.7  2008/09/30 10:20:49  marpas
#   last include MUST be the LAST !!!!
#
#   Revision 2.6  2008/06/30 15:31:52  enrcar
#   EC:: HW-cypering is now supported (TDI Matrix). A further layer (Operation03) has been added.
#
#   Revision 2.5  2008/06/04 15:11:25  enrcar
#   EC:: fileOperation01, fileOperation01Factory added
#
#   Revision 2.4  2008/03/20 18:14:11  clanas
#   fixed warning and added dependency on libCRC
#
#   Revision 2.3  2008/02/20 15:54:35  enrcar
#   EC:: mountlist.c file added
#
#   Revision 2.2  2007/04/13 17:17:51  marpas
#   new class to compute md5 in a separate thread
#
#   Revision 2.1  2006/08/23 14:49:35  paoscu
#   Shared libraries dependencies added
#
#   Revision 2.0  2006/02/28 08:33:12  marpas
#   Exception 2.1 I/F adopted
#
#   Revision 1.11  2006/02/14 13:04:59  enrcar
#   Byteswapping macros moved in a separate file
#
#   Revision 1.10  2005/12/05 16:52:09  paoscu
#   PROJECTFLAGS automatically evaluated by uppercasing $PROJECT
#
#   Revision 1.9  2005/11/03 10:13:53  marpas
#   USERFLAG used conditioned to $Project variable
#
#   Revision 1.8  2005/11/03 09:40:16  marpas
#   new class CThread added
#
#   Revision 1.7  2005/11/03 08:39:05  marpas
#   Operation01Factory added
#
#   Revision 1.6  2005/11/02 09:44:49  enrcar
#   Hardware-scan class (and related test) added
#
#   Revision 1.5  2005/10/28 16:35:01  enrcar
#   Operation02 package added
#
#   Revision 1.4  2005/09/06 08:43:27  marpas
#   new FileGuard class added
#
#   Revision 1.3  2004/04/21 09:44:59  marpas
#   new class added
#
#   Revision 1.2  2003/01/31 15:18:05  paoscu
#   LIB_INTEGRATION_PATH variable added
#
#   Revision 1.1.1.1  2002/10/04 16:58:31  paoscu
#   Import libFile
#
#
#
#

include ../../common/commondefs.mk

  PROJECTFLAG = -D$(shell echo $(PROJECT) | tr a-z A-Z)
  	
  USERFLAG += $(PROJECTFLAG)

  LIBRARYNAME =	libFile

  VERSIONFILE = lfV.C

     CSOURCES =	mountlist.c \
      		    ACScpuid.c 

     CINCLUDE =	

   C++SOURCES = \
			Cache.C \
			ChDir.C \
			CThread.C \
			Dir.C \
			DiskEventNotifier.C \
			File.C \
			FileGuard.C \
			FileMD5Th.C \
			HWScan.C \
			Network.C \
			Notify.C \
			Operation03.C \
			Operation02.C \
			Operation01.C \
			Operation01Factory.C \
			Password.C \
			FSTree.C \
			DigestTree_base.C \
			Uuid.C \
			lfV.C

   C++INCLUDE = \
			ACS_ByteSwap.h \
			Cache.h \
			ChDir.h \
			Dir.h \
			DiskEventNotifier.h \
			File.h \
			FileGuard.h \
			FileMD5Th.h \
			HWScan.h \
			Network.h \
			Notify.h \
			Operation03.h \
			Operation02.h \
			Operation01.h \
			Operation01Factory.h \
			Password.h \
			FSTree.h \
			DigestTree_base.h \
			DigestTree_sha1.h \
			DigestTree_md5.h \
			Uuid.h

COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#DSODEPENDLIBS =  -lStringUtils -lACS_SSL  -lCRC -lException  -lThread  -lTimer -lRegExp 

DSOSPECIFICDEPS = -luuid
#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

# DO NOT DELETE
include ../../common/commonrules.mk



