// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Scheduler $

	$Id$

	$Author$

	$Log$
	Revision 1.5  2012/02/09 18:19:48  marpas
	refactoring in progress
	
	Revision 1.4  2004/12/03 17:26:01  marfav
	Header Fixed
	
	Revision 1.3  2003/02/03 16:49:52  marfav
	*** empty log message ***
	
	Revision 1.1.1.1  2003/02/03 15:56:22  marfav
	Import Scheduler
	
	
	

*/

#ifndef _tlsV_H_
#define _tlsV_H_

namespace acs {

//! \brief support class for versioning




class tlsV { // PRQA S 2109
public:
	tlsV() throw() ;
	~tlsV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
    tlsV(const tlsV &) ; // not implemented
    tlsV& operator= (const tlsV &) ; // not implemented
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _tlsV_H_
