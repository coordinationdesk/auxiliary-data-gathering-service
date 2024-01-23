// PRQA S 1050 EOF
/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
	
*/

#ifndef _dgenV_H_
#define _dgenV_H_

namespace acs {

//! \brief support class for versioning


class dgenV { // PRQA S 2109
public:
	dgenV() ;
	~dgenV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	dgenV(const dgenV &) ; // not implemented 
	dgenV& operator= (const dgenV &) ; // not implemented 

private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dgenV_H_
