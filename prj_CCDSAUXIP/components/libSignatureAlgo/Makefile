#
#	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Makefile for libSignatureAlgo $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.3  2017/05/24 08:24:22  lucio.pulvirenti
#	CSGACQ-121: libAbstractFactoryChain added to dependent libs.
#	
#	Revision 2.2  2007/04/13 17:40:35  marpas
#	threaded md5 signature added
#	
#	Revision 2.1  2006/08/23 14:51:46  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 09:12:12  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
#	First Import
#		
#		
#

include ../../common/commondefs.mk

  LIBRARYNAME =	libSignatureAlgo

  VERSIONFILE = saV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		saV.C \
		SignatureAlgo.C \
		SignatureAlgoFactory.C \
		Md5Sum.C \
		Md5SumTh.C \
		
		

   C++INCLUDE = \
		SignatureAlgo.h \
		Md5Sum.h \
		Md5SumTh.h \
		SignatureAlgoFactory.h \
		SignatureAlgoScopeGuard.h \
		

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:
	@rm -f *.bak;


#empty rule, do not remove, you can add specific remotion commands
helpspecific:



# DO NOT DELETE


