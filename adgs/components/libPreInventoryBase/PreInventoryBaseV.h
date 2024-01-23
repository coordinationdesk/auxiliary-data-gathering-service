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

	$Prod: A.C.S. libPreInventoryBase$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/02/21 13:08:11  marpas
	refactoring
	
	Revision 1.1.1.1  2008/09/30 14:35:50  nicvac
	Import libPreInventoryBase
	
        
*/

#ifndef _PreInventoryBaseV_H_
#define _PreInventoryBaseV_H_

namespace acs {

   //! \brief support class for versioning

class PreInventoryBaseV { // PRQA S 2109
public:
    PreInventoryBaseV() throw() ;
    ~PreInventoryBaseV() throw() ;
    static const char*  tag() throw() ;
    static const char*  name() throw() ;
private:
    PreInventoryBaseV(const PreInventoryBaseV &) ; // not implemented
    PreInventoryBaseV& operator= (const PreInventoryBaseV &) ; // not implemented
private:
    static const char* 	_tag ;
    static const char* 	_name ;
} ;

} // Close namespace

#endif 


