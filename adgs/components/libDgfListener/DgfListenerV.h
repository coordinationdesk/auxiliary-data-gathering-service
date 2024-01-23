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

	$Prod: A.C.S. lV Library $

	$Name$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2012/02/13 17:04:55  marpas
	refactoring in progress
	
	Revision 1.3  2005/03/25 10:11:23  serfol
	modified the messages as exFilterable and not as ostringstream
	
	Revision 1.2  2003/07/02 10:41:49  serfol
	deleted ctor inline from include
	
	Revision 1.1.1.1  2002/12/18 17:35:38  luipir
	import libDgfListener start
	
	
	

*/

 
#ifndef _DgfListenerV_H_
#define _DgfListenerV_H_

namespace acs {

// support class for versioning

class DgfListenerV { // PRQA S 2109
public:
	DgfListenerV() throw() ;
	~DgfListenerV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	DgfListenerV(const DgfListenerV &) ; // not implemented
	DgfListenerV& operator=(const DgfListenerV &) ; // not implemented
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _DgfListenerV_H_



