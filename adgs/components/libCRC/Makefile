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

include ../../common/commonrules.mk

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:
