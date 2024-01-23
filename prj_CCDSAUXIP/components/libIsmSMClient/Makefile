##
#    Copyright 1995-2013, Advanced Computer Systems ,Inc.
#    Via Della Bufalotta, 378 - 00139 Roma - Italy
#    http://www.acsys.it
#    All Rights Reserved.
#
#    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#    the contents of this file may not be disclosed to third parties, copied or
#    duplicated in any form, in whole or in part, without the prior written
#    permission of Advanced Computer Systems, Inc.
#
#    $Prod: A.C.S Ism SM client library $
#
#    $Id$
#
#    $Author$
#
#    $Log$
#    Revision 6.0  2013/09/20 08:03:34  marpas
#    new t_inventorysm schema supported
#
#    Revision 5.0  2013/06/20 13:37:44  marpas
#    adopting libException 5.x standards
#    coding best practices applied
#    qa rules
#
#    Revision 1.4  2013/03/08 18:13:58  marpas
#    libDbBasic no longer used
#
#    Revision 1.3  2009/06/17 09:18:52  marpas
#    custom attributes for ISM: got from applicationName if any
#
#    Revision 1.2  2008/10/07 15:54:58  crivig
#    porting to postgres8
#
#    Revision 1.1.1.1  2008/06/16 15:14:58  marpas
#    Importing libIsmSMClient
#
#
#
#
#
include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)

  LIBRARYNAME =	libIsmSMClient

  VERSIONFILE = ismclientV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		ISMInterfaceFactoryMethod.C \
		ISMStorageManager.C \
		ismclientV.C

   C++INCLUDE = ISMStorageManager.h
	

DSOLIBRARYPATH = \
				-L$(LIB_INTEGRATION_PATH)/shared \
				-L$(LIB_INTEGRATION_PATH)/lib

DSOSPECIFICDEPS = -lApp -lIsmSoapClient -lSignatureAlgo

TARGETFILES=$(DSO_LIBRARY) 

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


#empty rule, do not remove, you can add specific help strings
helpspecific:


include ../../common/commonrules.mk



# DO NOT DELETE


