// PRQA S 1050 EOF
/*

	Copyright 2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DCUtility Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/12/10 11:21:23  marpas
	qa warning
	
	Revision 1.1  2012/02/28 17:16:17  micmaz
	refactory Data circulator
	
	

*/

#ifndef _dcutilityV_H_
#define _dcutilityV_H_

namespace acs {

// support class for versioning

class dcutilityV { // PRQA S 2109
public:
	dcutilityV() throw() ;
	~dcutilityV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	dcutilityV(const dcutilityV &) throw() ;
	const dcutilityV &operator=(const dcutilityV &) throw() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;
} // Close namespace

#endif // _dcutilityV_H_

