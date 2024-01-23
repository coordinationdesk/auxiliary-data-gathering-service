// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Algo Library $

	$Id$

	$Author$
	
	$Log$
	Revision 3.0  2013/09/23 12:58:27  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 2.1  2013/02/12 12:41:47  marpas
	dbConnectionPool concept introduced
	coding best practices added
	redundant functions (specially dealing with strings) replaced with StringUtils
	qa rules
	
	Revision 2.0  2006/02/28 09:13:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2004/09/30 09:34:56  ivafam
	Import libRollingAlgo
	
		
*/ 

#ifndef _rlV_H_
#define _rlV_H_

namespace acs {

// support class for versioning

class rlV { // PRQA S 2109
public:
	rlV() ;
	~rlV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	rlV(const rlV &) ; // not implemented
	rlV& operator= (const rlV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _rlV_H_

