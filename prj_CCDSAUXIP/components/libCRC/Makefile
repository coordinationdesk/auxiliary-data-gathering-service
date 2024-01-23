#
#	Copyright 1995-2022, Exprivia SpA - DFDA-AS
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia SpA
#
#	$Author$
#
#	$Prod: Multi-platform makefile $
#
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libCRC

  VERSIONFILE = crcV.C

     CSOURCES =	crcmodel.c crclut.c

     CINCLUDE = crcmodel.h crclut.h

   C++SOURCES = CRC_Digest_base.C crcV.C \
                PasswordHash.C

   C++INCLUDE = CRC_Digest_base.h  \
                CRC_Digest_sha1.h \
                CRC_Digest_md4.h \
                CRC_Digest_md5.h \
                PasswordHash.h

     DSOSPECIFICDEPS = -lcrypto -lssl -lcrypt



COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

include ../../common/commonrules.mk
