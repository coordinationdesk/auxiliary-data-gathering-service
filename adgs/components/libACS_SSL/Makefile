#
#	Copyright 1995-2019, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. SSL Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Release 3.2.2  2019/10/28 15:58:40  enrico.carta@acsys.it
#	EC:: LP:: Add a compile-time check to verify if the salt defines both occur: this would raise a compiling error.
#	
#	Revision 2.4  2014/05/06 16:45:19  marfav
#	Addedd global initialization support for crypto engines
#	
#	Revision 2.3  2008/09/30 14:10:22  lucio.pulvirenti
#	DSODEPENDLIBS MUST be BEFORE the last include
#	
#	Revision 2.2  2008/02/11 18:56:15  paoscu
#	-lssl added to DSODEPENDLIBS
#	
#	Revision 2.1  2006/08/23 14:50:58  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:32:29  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.1.1.1  2005/11/02 11:20:22  enrcar
#	Import source
#	
#

include ../../common/commondefs.mk

# The compile-time defines for setting the salt are: CIPHER_SALT_RANDOMSIZE and CIPHER_SALT_STRING (mutually exclusive)
# Example 1: random salt of 6 chars:
# OTHERFLAGS := $(OTHERFLAGS) -DCIPHER_SALT_RANDOMSIZE=6 
# Example 2: fixed string (please note the "DOUBLE QUOTES" ):
# OTHERFLAGS := $(OTHERFLAGS) -DCIPHER_SALT_STRING="ANY_STRING"
# Example 3: no salt:
# OTHERFLAGS := $(OTHERFLAGS) 
# Example 4: conflicting definition (this will raise an error) :
# OTHERFLAGS := $(OTHERFLAGS) -DCIPHER_SALT_RANDOMSIZE=6 -DCIPHER_SALT_STRING="ANY_STRING"


  LIBRARYNAME =	libACS_SSL

  VERSIONFILE = sslV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
				ACS_SSL.C \
				CryptoEngineInitializer.C \
				sslV.C

   C++INCLUDE = \
				CryptoEngineInitializer.h \
				ACS_SSL.h 




COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 

DSOSPECIFICDEPS = -lssh2 -lssl -lcrypto -ldl


# The -DFTPCURL_USE_GNUTLS forces the SSL initializer to make use of libgcrypt instead of libssl
ifneq (,$(findstring FTPCURL_USE_GNUTLS,$(OTHERFLAGS)))
   DSOSPECIFICDEPS += -lgcrypt
endif

include ../../common/commonrules.mk


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:




# DO NOT DELETE


