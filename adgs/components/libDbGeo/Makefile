#
#	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
#	$Prod: A.C.S. Database Geo Object Library $
#
#	$Id$
#
#	$Author$
#
#	$Log$
#	Revision 2.5  2013/03/29 18:03:21  marpas
#	libGraphicAttributes deprecated
#	
#	Revision 2.4  2012/12/14 11:13:41  marpas
#	coding best practices
#	qa rules
#	test improeved
#	test coverage
#	
#	Revision 2.3  2012/02/13 11:15:39  marpas
#	refactoring in progress
#	
#	Revision 2.2  2010/04/09 17:23:21  marpas
#	rules applied in preparation for ORACLE SPATIAL porting
#	
#	Revision 2.1  2006/08/23 14:50:51  paoscu
#	Shared libraries dependencies added
#	
#	Revision 2.0  2006/02/28 08:43:59  marpas
#	Exception 2.1 I/F adopted
#	
#	Revision 1.8  2005/10/22 19:26:14  marpas
#	dbGeoObject  parser implemented
#	
#	Revision 1.7  2005/08/09 13:33:06  marpas
#	DBEXPRESS alignement
#	
#	Revision 1.6  2005/07/06 14:40:13  enrcar
#	DBEXPRESS option added
#	
#	Revision 1.5  2005/06/20 16:28:33  enrcar
#	INCLUDEPATH REMOVED
#	
#	Revision 1.4  2003/05/22 09:52:32  marpas
#	new class added
#	
#	Revision 1.3  2003/03/03 14:22:15  marpas
#	fixed
#	
#	Revision 1.2  2003/01/31 15:14:04  paoscu
#	DB lock added
#	
#	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
#	Import libDbGeo
#	

include ../../common/commondefs.mk


  LIBRARYNAME =	libDbGeo

  VERSIONFILE = dbgV.C

     CSOURCES =	

     CINCLUDE =	

   C++SOURCES = \
		dbGeoPnt.C \
		dbScenePnt.C \
		dbQLookPnt.C \
		dbGeoPntList.C \
		dbGeoPointSortedList.C \
		dbGeoObject.C \
		dbGeoObjectGuard.C \
		dbGeoAltitude.C \
		dbGeoAltitudeRange.C \
		dbGeoTimeRange.C \
		dbGeoPoint.C \
		dbGeoLineseg.C \
		dbGeoString.C \
		dbGeoBox.C \
		dbGeoEllipse.C \
		dbGeoCircle.C \
		dbGeoRing.C \
		dbGeoPolygon.C \
		dbGeoStation.C \
		dbGeoStationSet.C \
		dbgV.C

   C++INCLUDE = \
		dbGeoPnt.h \
		dbScenePnt.h \
		dbQLookPnt.h \
		dbGeoPntList.h \
		dbGeoPointSortedList.h \
		dbGeoObject.h  \
		dbGeoObjectGuard.h  \
		dbGeoPoint.h \
		dbGeoLineseg.h \
		dbGeoString.h \
		dbGeoBox.h \
		dbGeoEllipse.h \
		dbGeoPolygon.h \
		dbGeoRing.h \
		dbGeoCircle.h \
		dbGeoAltitude.h \
		dbGeoAltitudeRange.h \
		dbGeoTimeRange.h \
		dbGeoStation.h \
		dbGeoStationSet.h

DSOLIBRARYPATH = -L$(LIB_INTEGRATION_PATH)/shared 


COV_PATHS := $(shell pwd)/test

COV_BASE_DIR := $(shell pwd)




include ../../common/commonrules.mk
DSOSPECIFICDEPS =



#empty rule, do not remove, you can add specific remotion commands
cleanspecific:


#empty rule, do not remove, you can add specific remotion commands
helpspecific:




# DO NOT DELETE
