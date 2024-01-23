/*

	Copyright 2002, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Product Header Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 09:14:07  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 09:58:39  giucas
	Header fixed
	
	Revision 1.1.1.1  2002/10/23 10:04:58  giucas
	Import libProductHeader
	
	


*/

#ifndef _prodHeaderV_H_
#define _prodHeaderV_H_

#include <acs_c++config.hpp>
_ACS_BEGIN_NAMESPACE(acs)

// support class for versioning

class prodHeaderV {
public:
	prodHeaderV() ;
	~prodHeaderV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _prodHeaderV_H_

