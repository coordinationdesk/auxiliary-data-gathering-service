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
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.5  2013/03/29 19:50:57  marpas
	removing html related functions
	
	Revision 2.4  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.3  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
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
	
	Revision 1.9  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.8  2004/08/31 09:55:30  marpas
	unuset method removed
	
	Revision 1.7  2004/06/18 10:19:03  marpas
	new method
	
	Revision 1.6  2003/05/26 23:54:48  marpas
	Graphic attributes management almost complete
	
	Revision 1.5  2003/05/26 16:46:34  marpas
	attributes management
	
	Revision 1.4  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.3  2003/01/28 17:32:29  paoscu
	Interface modified
	
	Revision 1.2  2003/01/28 15:23:56  paoscu
	inheritance chain slightly modified
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/


#ifndef _dbStObj_H_
#define _dbStObj_H_ 

#include <acs_c++config.hpp>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbGeoObject ;
class dbQLookPnt ;
class dbGeoPoint ;

class dbStObj : 
	public dbPersistent
{	
public:

	dbStObj() ;
	virtual ~dbStObj() throw() ;

	virtual const dbGeoObject *shape() const = 0 ;
	
	virtual void scene2Geo(const dbQLookPnt &, dbGeoPoint &) ;
	virtual void geo2Scene(const dbGeoPoint &, dbQLookPnt &) ;
	virtual void qlLineNumber(long) ;
	
	size_t id() const ;
	void id(size_t) ;
protected:
	dbStObj(const dbStObj &) ;
	dbStObj &operator=(const dbStObj &) ; 
} ;

_ACS_END_NAMESPACE

#endif

