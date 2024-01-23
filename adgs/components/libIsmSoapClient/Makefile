#
#	Copyright 1995-2021, Exprivia SpA - DADF
#	Via Della Bufalotta, 378 - 00139 Roma - Italy
#	http://www.exprivia.com
#
#	All Rights Reserved.
#
#	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#	the contents of this file may not be disclosed to third parties, copied or
#	duplicated in any form, in whole or in part, without the prior written
#	permission of Exprivia, SpA 
#
#	$Prod: A.C.S. libIsmSoapClient$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 5.0  2013/07/22 18:15:54  marpas
#	adopting libException 5.x standards
#	
#	Revision 1.14  2012/04/16 15:25:36  lucio.pulvirenti
#	libApp dependance added
#	
#	Revision 1.13  2012/03/27 10:30:23  marpas
#	makefiles fixed
#	
#	Revision 1.12  2012/02/22 09:50:21  marpas
#	no more warnings in compilation
#	
#	Revision 1.11  2011/03/02 16:11:42  lucio.pulvirenti
#	libcurl dependance removed
#	
#	Revision 1.10  2011/03/01 17:47:46  marpas
#	work in progress
#	
#	Revision 1.9  2010/09/22 09:21:22  marpas
#	nothing important
#	
#	Revision 1.8  2009/06/19 10:17:19  lucio.pulvirenti
#	Added dependency om libACS_SSL
#	
#	Revision 1.7  2009/06/16 14:53:11  marpas
#	new client interface for parameters, older one still valid
#	multiport(2) attempts to connect the server (if port2 set)
#	.so dependencies changed (new types used)
#	
#	Revision 1.6  2008/11/18 17:03:05  paoscu
#	Tests building forced
#	
#	Revision 1.5  2008/07/04 16:38:17  paoscu
#	Enhanced
#	
#	Revision 1.4  2008/06/16 17:23:18  paoscu
#	Makefile modified to work with parallel execution
#	
#	Revision 1.3  2008/05/29 17:56:33  paoscu
#	Added FTP support
#	
#	Revision 1.2  2008/05/28 17:32:25  paoscu
#	Added download methods
#	
#	Revision 1.1.1.1  2008/05/07 17:48:45  paoscu
#	Imported into CVS
#	
#        
#


include ../../common/commondefs.mk

# This force to compile all test after the Library has been compiled
   POSTCOMPILERULE += test

   LIBRARYNAME = libIsmSoapClient

   VERSIONFILE = IsmSoapClientV.C

   C++SOURCES =\
    IsmSoapClient.C \
	IsmSoapClientV.C \
	PackagingAlgoFactory.C \
	PackagingAlgoSingleton.C \
	PackagingAlgo.C \
	TarAlgo.C \
	TarGzAlgo.C \
	IsmServerVersionManagerSingleton.C \


   C++INCLUDE = IsmSoapClient.h \
	PackagingAlgoFactory.h \
	PackagingAlgoSingleton.h \
	PackagingAlgo.h \
	TarAlgo.h \
	TarGzAlgo.h \
	IsmServerVersionManagerSingleton.h \


test: $(TARGETFILES)

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:


# DO NOT DELETE

include ../../common/commonrules.mk

