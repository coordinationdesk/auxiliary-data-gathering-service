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

	$Prod: A.C.S. Plugin Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.2  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.1  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.1.1.1  2002/10/29 18:24:47  paoscu
	Import libPlugin
	
	

*/

#ifndef _plV_H_
#define _plV_H_

namespace acs {

// support class for versioning

class plV { // PRQA S 2109
public:
	plV() throw() ;
	~plV() throw() ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
    plV(const plV &) ;  // not implemented
    plV & operator= (const plV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _plV_H_

