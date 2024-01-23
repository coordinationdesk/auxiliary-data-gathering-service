// PRQA S 1050 EOF
/*
	Copyright 1995-2008, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libacdbexppgl $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2008/08/19 09:18:13  enrcar
	EC:: typo fixed
	
	
        
*/

#ifndef _acdbexppglV_H_
#define _acdbexppglV_H_

   //! \brief support class for versioning

   class acdbexppglV { // PRQA S 2109
   public:
   	acdbexppglV() ;
   	~acdbexppglV() ;
   	static const char*  tag() { return _tag ; }
   	static const char*  name() { return _name ; }
   private:
   	acdbexppglV(const acdbexppglV &) ; // not implemented
   	acdbexppglV& operator= (const acdbexppglV &) ; // not implemented
   private:
   	static const char* 	_tag ;
   	static const char* 	_name ;
   } ;

#endif 


