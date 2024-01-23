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

	$Prod: A.C.S. libGRIBPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/10/18 15:07:33  giufor
	Import libGRIBPreInventory
	
	
        
*/

#ifndef _GRIBPreInventoryV_H_
#define _GRIBPreInventoryV_H_

namespace acs {

//! \brief support class for versioning

class GRIBPreInventoryV { // PRQA S 2109
public:
	GRIBPreInventoryV() ;
	~GRIBPreInventoryV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	GRIBPreInventoryV(const GRIBPreInventoryV &) ; // not implemented 
	GRIBPreInventoryV & operator= (const GRIBPreInventoryV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif 


