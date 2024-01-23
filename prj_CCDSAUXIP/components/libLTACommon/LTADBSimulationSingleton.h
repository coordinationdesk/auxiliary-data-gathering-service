// PRQA S 5600, 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA Common functions $


*/


#ifndef _LTADBSimulationSingleton_H_
#define _LTADBSimulationSingleton_H_ 

#include <acs_c++config.hpp>
#include <exStream.h>
#include <Singleton.hpp>


_ACS_BEGIN_NESTED_NAMESPACE (acs, lta)

//
//
// class LTADBSimulationSingleton
//
//

using pattern::Singleton ;

class LTADBSimulationSingleton: public Singleton<LTADBSimulationSingleton > // PRQA S 2109, 2153
{
	friend LTADBSimulationSingleton* Singleton< LTADBSimulationSingleton >::instance(); // PRQA S 2107
public:
	virtual ~LTADBSimulationSingleton() throw() ; 

	bool simulation() const ; // return true if in simulation mode
	void simulation(bool) ;

protected:
	LTADBSimulationSingleton() ; 
	
private:
	LTADBSimulationSingleton &operator=(const LTADBSimulationSingleton &) ; 	// declared but not implemented
	LTADBSimulationSingleton(const LTADBSimulationSingleton &) ; 				// declared but not implemented

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTADBSimulationSingleton) ;
} ; 

_ACS_END_NESTED_NAMESPACE


#endif // _LTADBSimulationSingleton_H_

