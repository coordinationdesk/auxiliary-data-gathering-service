/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DIWorkOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 09:42:37  giucas
	Header fixed
	
	Revision 1.1.1.1  2002/11/26 14:11:20  clanas
	imported source
	
	Revision 1.1.1.1  2002/09/30 19:36:15  marpas
	Import libException
	

*/

#ifndef _disWorkOrderV_H_
#define _disWorkOrderV_H_

#include <acs_c++config.hpp>
_ACS_BEGIN_NAMESPACE(acs)


class disWorkOrderV {
public:
	disWorkOrderV() ;
	~disWorkOrderV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _disWorkOrderV_H_

