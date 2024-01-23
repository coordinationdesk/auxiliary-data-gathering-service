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
#	$Prod: A.C.S. Makefile for libDbSpace $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 1.16  2013/03/29 19:51:08  marpas
#	removing html related functions
#	
#	Revision 1.15  2013/03/08 16:59:36  marpas
#	libDbBasic no longer used
#	
#	Revision 1.14  2013/02/28 13:22:12  marpas
#	no libMessage dependency
#	
#	Revision 1.13  2012/11/19 14:16:56  marpas
#	HEAD REALIGNEMENT
#	
#	Revision 1.11  2008/10/07 13:10:51  crivig
#	porting to postgres8
#	
#	Revision 1.10  2006/08/23 14:50:03  paoscu
#	Shared libraries dependencies added
#	
#	Revision 1.9  2005/10/24 16:45:53  marpas
#	dbexpress compatibility
#	
#	Revision 1.8  2005/06/17 15:55:44  enrcar
#	INCLUDEPATH Removed
#	
#	Revision 1.7  2004/04/13 15:05:16  marpas
#	ace dependencies removed
#	dbPersistent::clone method implemented
#	dbGeneralQuery::clone method implemented
#	XXXQuery::operator= implemented
#	
#	Revision 1.6  2003/03/03 14:17:54  marpas
#	fixed
#	
#	Revision 1.5  2003/02/03 18:26:19  marpas
#	LIB_INTEGRATION_PATH variable management
#	
#	Revision 1.4  2003/01/16 19:07:10  paoscu
#	makefile updated
#	
#	Revision 1.3  2002/11/13 12:54:31  danalt
#	ACE and HSM lib and include added
#	
#	Revision 1.2  2002/11/06 13:28:57  danalt
#	allineamento con dbCryosat
#	
#	Revision 1.1.1.1  2002/10/08 12:36:28  marpas
#	libDbSpace import
#		
#

include ../../common/commondefs.mk

#dbexpress compatibility mode (for Informix w/ SQLString)
#CUSTOMFLAGS:=-DDBEXPRESS_COMPAT $(CUSTOMFLAGS)

  LIBRARYNAME =	libDbSpace

  VERSIONFILE = dbspV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		dbspV.C \
		dbStation.C \
		dbStationQuery.C \
		dbSatellite.C \
		dbSatelliteQuery.C \
		dbSensor.C \
		dbSensorQuery.C \
		dbSatRelation.C \
		dbSatRelationQuery.C \
		dbSpaceFactory.C \


   C++INCLUDE = \
		dbStation.h \
		dbStationQuery.h \
		dbSatellite.h \
		dbSatelliteQuery.h \
		dbSensor.h \
		dbSensorQuery.h \
		dbSatRelation.h \
		dbSatRelationQuery.h \
		dbSpaceFactory.h \


DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:

helpspecific:



# DO NOT DELETE


