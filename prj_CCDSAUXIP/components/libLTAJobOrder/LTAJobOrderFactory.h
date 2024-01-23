// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA JobOrder Library $

*/

#ifndef _LTAJobOrderFactory_H_
#define _LTAJobOrderFactory_H_

#include <exException.h>
#include <StringKeyFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

class JobOrder ;

class LTAJobOrderFactory : public StringKeyFactory <JobOrder>
{
public:
	LTAJobOrderFactory();
	static acs::JobOrder* buildLTAJobOrder();

private:
	LTAJobOrderFactory (const LTAJobOrderFactory&);
	LTAJobOrderFactory& operator=(const LTAJobOrderFactory&);
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAJobOrderFactory);
};

_ACS_END_NAMESPACE

#endif //_LTAJobOrderFactory_H_

