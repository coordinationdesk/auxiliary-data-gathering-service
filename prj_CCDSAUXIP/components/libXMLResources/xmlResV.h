// PRQA S 1050 EOF
/*

	Copyright 2009-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. xmlResVources Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.2  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.1  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/06 14:09:23  marpas
	header fixed
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources

*/

#ifndef _xmlResV_H_
#define _xmlResV_H_

namespace acs {

// support class for versioning

class xmlResV { // PRQA S 2109
public:
	xmlResV() throw() ;
	~xmlResV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	//declared but not implemented to prevent their use
	xmlResV(const xmlResV &) ;
	xmlResV & operator=(const xmlResV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _xmlResV_H_

