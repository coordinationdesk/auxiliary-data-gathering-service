// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.5  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.4  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.2  2012/02/13 16:51:17  marpas
	refactoring in progress
	
	Revision 2.1  2006/10/16 10:46:32  marpas
	Drawable concept introduced in libGraphicAttributes, from which this library was already dependent
	dbPersistent is now a Drawable
	dbStObj Is a Drawable too because inheritance from dbPersistent and does not
	longer implements attributes related methods
	Factories dealing with Attributes are now moved to the more appropriate libGraphicAttributes
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.7  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.6  2003/05/26 23:54:48  marpas
	Graphic attributes management almost complete
	
	Revision 1.5  2003/05/26 16:46:34  marpas
	attributes management
	
	Revision 1.4  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.3  2003/04/17 13:58:42  marpas
	GCC3.2
	
	Revision 1.2  2003/01/28 15:23:56  paoscu
	inheritance chain slightly modified
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#include <dbStObj.h>
#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)

dbStObj::dbStObj() :
    dbPersistent() // PRQA S 4052
{
}

dbStObj::~dbStObj() throw() {}


dbStObj::dbStObj(const dbStObj &v) : // copy ctor
	dbPersistent(v) // PRQA S 4052
{
}

dbStObj &dbStObj::operator=(const dbStObj &v) 
{ 
	if(this != &v) { dbPersistent::operator=(v) ; }
	return *this ;
} 


void dbStObj::scene2Geo(const dbQLookPnt &, dbGeoPoint &) {}
void dbStObj::geo2Scene(const dbGeoPoint &, dbQLookPnt &) {}
void dbStObj::qlLineNumber(long line) {}


_ACS_END_NAMESPACE
