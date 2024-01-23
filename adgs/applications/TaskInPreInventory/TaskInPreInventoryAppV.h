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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2014/02/17 13:48:01  marpas
	coding best practices applied
	
	Revision 1.1.1.1  2012/11/30 12:44:54  chicas
	Imported sources
	
	
        
*/

#ifndef _TaskInPreInventoryAppV_H_
#define _TaskInPreInventoryAppV_H_

namespace acs {

   //! \brief support class for versioning

   class TaskInPreInventoryAppV { // PRQA S 2109
   public:
   	TaskInPreInventoryAppV() ;
   	~TaskInPreInventoryAppV() ;
   	static const char*  tag() { return _tag ; }
   	static const char*  name() { return _name ; }
   private:
   	TaskInPreInventoryAppV(const TaskInPreInventoryAppV & ) ; // not implemented
   	TaskInPreInventoryAppV& operator= (const TaskInPreInventoryAppV & ) ; // not implemented
   private:
   	static const char* 	_tag ;
   	static const char* 	_name ;
   } ;

} // Close namespace

#endif 


