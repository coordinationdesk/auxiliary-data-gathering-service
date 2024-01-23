// PRQA S 1050 EOF
/*
	Copyright 2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageTask$

	$Id$

	$Author$

	$Log$

*/

#ifndef _XMLInitializer_H_
#define _XMLInitializer_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs)

class XMLInitializer : public pattern::Singleton<XMLInitializer> // PRQA S 2109, 2153
{
    friend XMLInitializer* pattern::Singleton<XMLInitializer>::instance(); // PRQA S 2107

public:
    virtual ~XMLInitializer() throw(); // PRQA S 2131
    
protected:
    XMLInitializer();
    
private:
    XMLInitializer (const XMLInitializer&);
    XMLInitializer& operator= (const XMLInitializer&);
};
    
_ACS_END_NAMESPACE

#endif //_XMLInitializer_H_
