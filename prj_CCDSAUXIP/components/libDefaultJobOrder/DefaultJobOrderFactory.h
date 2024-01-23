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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/10/17 13:11:09  chicas
	imported sources
	

*/

#ifndef _DefaultJobOrderFactory_H_
#define _DefaultJobOrderFactory_H_

#include <acs_c++config.hpp>
#include <JobOrderFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class DefaultJobOrderFactory : public StringKeyFactory <JobOrder>
{
public:
	DefaultJobOrderFactory();
	virtual ~DefaultJobOrderFactory() throw() {} // PRQA S 2131
	static acs::JobOrder* buildDefaultJobOrder();

private:
	DefaultJobOrderFactory (const DefaultJobOrderFactory&);
	DefaultJobOrderFactory& operator= (const DefaultJobOrderFactory&);
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultJobOrderFactory) ;
};

_ACS_END_NAMESPACE

#endif //  _DefaultJobOrderFactory_H_
