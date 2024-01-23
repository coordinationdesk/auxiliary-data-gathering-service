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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/

#ifndef _ddrcV_H_
#define _ddrcV_H_

namespace acs {

//! \brief support class for versioning




class ddrcV { // PRQA S 2109
public:
	ddrcV() ;
	~ddrcV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	ddrcV(const ddrcV &) ; // not implemented
	ddrcV& operator= (const ddrcV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ddrcV_H_
