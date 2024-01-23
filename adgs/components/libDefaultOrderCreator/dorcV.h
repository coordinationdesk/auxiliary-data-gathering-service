/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Default Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.2  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.1  2012/03/07 09:42:42  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
		

*/

#ifndef _dorcV_H_
#define _dorcV_H_

namespace acs {

//! \brief support class for versioning




class dorcV { // PRQA S 2109
public:
	dorcV() throw() ;
	~dorcV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	dorcV(const dorcV & ) ; // not implemented
	dorcV& operator=(const dorcV & ) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dorcV_H_
