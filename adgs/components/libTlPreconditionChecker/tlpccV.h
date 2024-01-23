// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Precondition Checker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/02/10 17:24:44  marpas
	refactoring in progress
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	

*/

#ifndef _tlpccV_H_
#define _tlpccV_H_

namespace acs {

//! \brief support class for versioning




class tlpccV { // PRQA S 2109
public:
	tlpccV() throw() ;
	~tlpccV() throw() ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
	tlpccV(const tlpccV &) ;
	tlpccV& operator= (const tlpccV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tlpccV_H_
