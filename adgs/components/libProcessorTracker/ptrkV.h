// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processor Tracker Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:18:10  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:50:50  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/15 12:19:58  marfav
	Import libProcessorTracker
	

*/

#ifndef _ptrkV_H_
#define _ptrkV_H_

namespace acs {

//! \brief support class for versioning

class ptrkV { // PRQA S 2109
public:
	ptrkV() ;
	~ptrkV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	ptrkV(const ptrkV &) ; // not implemented 
	ptrkV& operator= (const ptrkV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ptrkV_H_
