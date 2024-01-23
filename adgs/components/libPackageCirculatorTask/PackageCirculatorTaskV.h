/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageCirculatorTask$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _PackageCirculatorTaskV_H_
#define _PackageCirculatorTaskV_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class PackageCirculatorTaskV {
public:
	PackageCirculatorTaskV();
	~PackageCirculatorTaskV();
	static const char*  tag() { return _tag; }
	static const char*  name() { return _name; }
private:
	static const char* 	_tag;
	static const char* 	_name;
};

_ACS_END_NAMESPACE

#endif 


