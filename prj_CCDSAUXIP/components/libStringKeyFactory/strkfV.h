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

	$Prod: A.C.S. Template StringKey Based Factory Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/07/07 17:36:31  marpas
	coding besta practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 1.2  2004/12/03 18:00:41  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:30:10  marfav
	Import libStringKeyFactory
	

*/

#ifndef _strkfV_H_
#define _strkfV_H_

namespace acs {

//! \brief support class for versioning




class strkfV { // PRQA S 2109
public:
	strkfV() ;
	~strkfV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	strkfV(const strkfV&) ; // not implemented 
	strkfV& operator= (const strkfV&) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _strkfV_H_
