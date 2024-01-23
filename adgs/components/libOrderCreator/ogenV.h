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

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/02/12 12:43:50  marpas
	dbConnectionPool enforced
	interface rationalization
	coding best practices applied
	qa rules
	
	Revision 2.3  2013/02/11 12:50:20  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.2  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	
	Revision 2.1  2009/04/10 13:22:02  marpas
	OrderCreator emits an exception if no orbit in GOCE
	some rules fixed
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:09:06  marfav
	Import libOrderCreator
	
		

*/

#ifndef _ogenV_H_
#define _ogenV_H_


namespace acs {

//! \brief support class for versioning




class ogenV { // PRQA S 2109
public:
	ogenV() throw() ;
	~ogenV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not implemented to prevent their use
	ogenV(const ogenV &) ;
	ogenV &operator=(const ogenV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ogenV_H_
