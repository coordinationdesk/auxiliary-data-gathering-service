#
#	Copyright 1995-2002, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. DateTime Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.8  2008/10/20 15:55:39  marpas
#	Makefile omogenization
#	
#	Revision 2.5  2008/10/08 09:21:37  marpas
#	include fixed
#	
#	Revision 2.4  2007/11/22 11:29:18  lucio.pulvirenti
#	-lFeatures added to libs.
#	
#	Revision 2.3  2007/02/21 15:46:22  marqua
#	added WRAPPERS
#	
#	Revision 2.2  2006/09/19 10:53:11  marpas
#	helpspecific target was missing
#	inclusion of commonrules should be last stmt of Makefile
#	
#	Revision 2.1  2006/08/23 14:50:55  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:40:49  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.5  2005/09/08 14:17:17  paoscu
#	INCLUDEPATH REMOVED
#	
#	Revision 1.4  2004/08/03 13:48:24  marfav
#	Added TimeRange class to libDateTime
#	
#	Revision 1.3  2003/06/10 09:30:00  marpas
#	new class added
#	
#	Revision 1.2  2003/01/31 14:58:08  paoscu
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
#	Import libDateTime
#	
#	
#	

include ../../../common/commondefs.mk

  LIBRARYNAME =	pyDateTime

  VERSIONFILE = pydtV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = pyDateTime.C \
   				pydtV.C
				

CUSTOM_INCLUDEPATH= -I/usr/include/python2.4 -I..
CUSTOMFLAGS= -DNDEBUG -Wp,-D_FORTIFY_SOURCE=2 -fexceptions -fstack-protector --param=ssp-buffer-size=4
#for insure++ && gcc 2.x
#ALLRULE = precompile compile 
DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 
DSODEPENDLIBS =    -lDateTime -lboost_python

#WRAPPERS = pyDateTime

#empty rule, do not remove, you can add specific help strings
helpspecific:


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


include ../../../common/commonrules.mk

# DO NOT DELETE


