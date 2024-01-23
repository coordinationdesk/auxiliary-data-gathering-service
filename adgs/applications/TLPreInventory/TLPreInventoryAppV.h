/*
	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libTLPreInventoryApp$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2008/10/17 14:21:04  nicvac
	Import TLPreInventory
	
        
*/

#ifndef _TLPreInventoryAppV_H_
#define _TLPreInventoryAppV_H_

namespace acs {

   //! \brief support class for versioning

   class TLPreInventoryAppV {
   public:
   	TLPreInventoryAppV() ;
   	~TLPreInventoryAppV() ;
   	static const char*  tag() { return _tag ; }
   	static const char*  name() { return _name ; }
   private:
   	static const char* 	_tag ;
   	static const char* 	_name ;
   } ;

} // Close namespace

#endif 


