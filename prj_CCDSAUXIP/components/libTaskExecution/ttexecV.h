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

	$Prod: A.C.S. TaskTableExecution Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2008/04/22 11:13:22  marfav
	Executors implemented using the conditioned visitor patter
	
*/

#ifndef _ttexecV_H_
#define _ttexecV_H_

namespace acs {

//! \brief support class for versioning




class ttexecV { // PRQA S 2109
public:
	ttexecV() ;
	~ttexecV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	ttexecV(const ttexecV &) ; // not implemented
	ttexecV& operator= (const ttexecV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ttexecV_H_
