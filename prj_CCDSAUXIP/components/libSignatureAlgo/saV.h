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

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	
	Revision 1.2  2005/06/07 09:56:56  integrator
	split in progress
	
	Revision 1.1.1.1  2005/05/18 07:57:07  marpas
	Importing libStorageManager
	
	

*/

#ifndef _saV_H_
#define _saV_H_

namespace acs {

// support class for versioning

class saV { // PRQA S 2109
public:
	saV() ;
	~saV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	saV(const saV &) ; // not implemented
	saV& operator= (const saV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _saV_H_

