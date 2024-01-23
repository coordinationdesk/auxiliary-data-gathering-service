/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Ocada JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2009/03/02 14:40:14  chicas
	Just imported
	

*/

#ifndef _OcadaJobOrderFactory_H_
#define _OcadaJobOrderFactory_H_


#include <JobOrderFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class OcadaJobOrderFactory : public StringKeyFactory <JobOrder>
{
	public:
		OcadaJobOrderFactory();
		static acs::JobOrder* buildOcadaJobOrder();

	private:
		OcadaJobOrderFactory (const OcadaJobOrderFactory&);
		OcadaJobOrderFactory& operator= (const OcadaJobOrderFactory&);
		ACS_CLASS_DECLARE_DEBUG_LEVEL(OcadaJobOrderFactory)
};

_ACS_END_NAMESPACE


#endif //  _OcadaJobOrderFactory_H_
