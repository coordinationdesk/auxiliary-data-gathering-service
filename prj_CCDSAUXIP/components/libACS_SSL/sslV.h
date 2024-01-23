// PRQA S 1050 EOF
/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SSL Library Ver_h $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 08:32:29  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/11/02 11:20:22  enrcar
	Import source
	

*/
 
#ifndef _sslV_H_
#define _sslV_H_

namespace acs {

// support class for versioning

class sslV { // PRQA S 2109
public:
	sslV() ;
	~sslV() ;
	static const char* tag() { return _tag ; }
	static const char* name() { return _name ; }
private: 
    sslV(const sslV &) ;
    sslV& operator= (const sslV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _sslV_H_



