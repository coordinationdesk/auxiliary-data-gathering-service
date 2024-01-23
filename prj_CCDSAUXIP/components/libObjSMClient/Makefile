##
#    Copyright 2020-2022, Exprivia SpA - DFDA-AS
#    Via Della Bufalotta, 378 - 00139 Roma - Italy
#    http://www.exprivia.com
#    All Rights Reserved.
#
#    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
#    the contents of this file may not be disclosed to third parties, copied or
#    duplicated in any form, in whole or in part, without the prior written
#    permission of Exprivia SpA
#
#
#
include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)

  LIBRARYNAME =	libObjSMClient

  VERSIONFILE = ObjSmV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		ObjSMInterfaceFactoryMethod.C \
		ObjDdlSMInterfaceFactoryMethod.C \
		ObjStorageManager.C \
		StorageFileNamingAlgoFactory.C \
		StorageFileNamingAlgoSingleton.C \
		StorageFileNamingAlgo.C \
		StorageFileNamingAlgoIdentity.C \
		StorageFileNamingAlgoPdsLike.C \
		ObjSmV.C

   C++INCLUDE = 
	

#DSOLIBRARYPATH = \
				-L$(LIB_INTEGRATION_PATH)/shared \
				-L$(LIB_INTEGRATION_PATH)/lib

   DSOSPECIFICDEPS =

#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


#empty rule, do not remove, you can add specific help strings
helpspecific:


include ../../common/commonrules.mk



# DO NOT DELETE


