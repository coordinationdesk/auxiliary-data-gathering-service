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

	$Prod: A.C.S. Cryosat File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:23  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.3  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.2  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	

	
	

*/

#ifndef _gfaV_H_
#define _gfaV_H_

namespace acs {

// support class for versioning

class gfaV { // PRQA S 2109
public:
	gfaV() throw() ;
	~gfaV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	gfaV(const gfaV &) ; // not implemented
	gfaV& operator= (const gfaV &) ; // not implemented
    
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _gfaV_H_

