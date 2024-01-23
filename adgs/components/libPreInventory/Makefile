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
#	$Prod: A.C.S. libPreInventory$
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.2  2018/10/04 13:45:51  marpas
#	compilation and dependencies fixed
#	
#	Revision 2.1  2017/03/27 07:38:31  marfav
#	Fixed dependencies on time converter library
#	
#	Revision 2.0  2016/11/21 16:13:43  marfav
#	Major release
#	
#	Revision 1.12  2013/10/11 11:19:50  chicas
#	Removed -lAuxFileHandlingDefault and -lOBTConverter
#	
#	Revision 1.11  2013/04/24 10:26:05  chicas
#	updated DSOPENDLIBS
#	
#	Revision 1.10  2012/12/17 10:48:33  chicas
#	added TaskInPreInventory
#	
#	Revision 1.9  2012/12/10 10:42:13  clanas
#	removed devel classes
#	
#	Revision 1.8  2012/12/10 10:16:48  chicas
#	 '
#	
#	Revision 1.7  2012/10/11 15:02:14  chicas
#	Added TaskPreInventory
#	
#	Revision 1.6  2009/04/20 15:21:18  nicvac
#	Propagator included.
#	
#	Revision 1.5  2008/10/23 10:49:54  nicvac
#	dso updated.
#	
#	Revision 1.4  2008/10/23 09:02:03  nicvac
#	Lib list updated.
#	
#	Revision 1.3  2008/10/13 14:48:34  nicvac
#	TlPreInventory implementation
#	
#	Revision 1.2  2008/10/10 16:19:19  nicvac
#	DSO added.
#	
#	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
#	Import libPreInventory
#	
#        
#

include ../../common/commondefs.mk

   LIBRARYNAME = libPreInventory

   VERSIONFILE = PreInventoryV.C

   CUSTOM_INCLUDEPATH =

   CUSTOM_LIBPATH =

   CSOURCES =	

   CINCLUDE =	

   C++SOURCES = \
	PreInventory.C \
	DisPreInventory.C \
	SingleProdPreInventory.C \
	TlPreInventory.C \
	TaskPreInventory.C \
	TaskInPreInventory.C \
	PreInventoryCommonApp.C \
	PreInventoryV.C \


   C++INCLUDE = \
	PreInventory.h \
	DisPreInventory.h \
	SingleProdPreInventory.h \
	TlPreInventory.h \
	TaskPreInventory.h \
	TaskInPreInventory.h \
	PreInventoryCommonApp.h \


#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

#empty rule, do not remove, you can add specific help strings
helpspecific:

DSOSPECIFICDEPS =

# DO NOT DELETE

include ../../common/commonrules.mk
