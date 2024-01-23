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
#	Authors: 
#
#	$Prod: A.C.S. Status Manager Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.1  2006/08/23 14:52:25  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 10:06:27  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.4  2005/09/08 14:18:18  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.3  2003/08/20 09:52:52  envisat
#	Changed commondefs and commonrules includes paths from /raid0/cryosat to ../../
#	
#	Revision 1.2  2003/03/11 18:28:39  marfav
#	Singleton interface added
#	
#	Revision 1.1.1.1  2003/03/05 15:12:59  marfav
#	Import libStatusMgr
#	
#	
#

include ../../common/commondefs.mk

  LIBRARYNAME = libStatusMgr

  VERSIONFILE = smV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		StatusMgr.C\
		smV.C\


   C++INCLUDE = \
		StatusMgr.h \
		StatusMgrSingleton.h\





DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


DSOSPECIFICDEPS =


#empty rule, do not remove, you can add specific remotion commands

cleanspecific:

helpspecific:

include ../../common/commonrules.mk

