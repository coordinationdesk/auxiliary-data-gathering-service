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

	$Prod: A.C.S. libPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/02/21 18:21:20  marpas
	interface redesign to allow dbConnectionPool and explicit dbConnections
	debug improved
	coding best practices applied
	qa rules
	test fixing
	
	Revision 1.2  2009/05/22 15:35:27  marpas
	some rules fixed
	
	Revision 1.1.1.1  2008/10/01 15:41:36  nicvac
	Import libPreInventory
	
        
*/

#ifndef _PreInventoryV_H_
#define _PreInventoryV_H_

namespace acs {

   //! \brief support class for versioning

class PreInventoryV { // PRQA S 2109
public:
   	PreInventoryV() ;
   	~PreInventoryV() throw() ;
   	static const char*  tag() ;
   	static const char*  name() ;
private:
   	PreInventoryV(const PreInventoryV &) ;
   	PreInventoryV &operator=(const PreInventoryV &) ;
   
private:
   	static const char* 	_tag ;
   	static const char* 	_name ;
} ;

} // Close namespace

#endif 


