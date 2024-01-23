// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Archive $

	$Id$

	$Author$
	
	$Log$
	Revision 3.0  2013/09/23 10:31:57  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 2.1  2012/02/09 17:59:54  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 16:45:47  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2004/09/10 16:26:01  ivafam
	Import RollingArchive
	
		
*/ 

#ifndef _raV_H_
#define _raV_H_

namespace acs {

// support class for versioning

class raV { // PRQA S 2109
public:
	raV()  throw() ;
	~raV()  throw() ;
	static const char* tag()  throw() ;
	static const char* name()  throw() ;
private:
    raV(const raV &) ; // not implemented 
    raV& operator=(const raV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _raV_H_

