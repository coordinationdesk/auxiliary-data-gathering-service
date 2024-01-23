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

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/11 12:50:19  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.2  2009/04/10 13:22:02  marpas
	OrderCreator emits an exception if no orbit in GOCE
	some rules fixed
	
	Revision 2.1  2007/04/18 13:34:11  marpas
	simulation can be forced
	
	Revision 2.0  2006/02/28 10:23:55  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:09:06  marfav
	Import libOrderCreator
	
		

*/


#ifndef _OGDBSimulationSingleton_H_
#define _OGDBSimulationSingleton_H_ 

#include <acs_c++config.hpp>
#include <exStream.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs)

//
//
// class OGDBSimulationSingleton
//
//

using pattern::Singleton ;

class OGDBSimulationSingleton: public Singleton<OGDBSimulationSingleton > // PRQA S 2109, 2153
{
	friend OGDBSimulationSingleton* Singleton< OGDBSimulationSingleton >::instance(); // PRQA S 2107
public:
	virtual ~OGDBSimulationSingleton() throw() ; 

	bool simulation() const ; // return true if in simulation mode
	void simulation(bool) ;

protected:
	OGDBSimulationSingleton() ; 
	
private:
	OGDBSimulationSingleton &operator=(const OGDBSimulationSingleton &) ; 	// declared but not implemented
	OGDBSimulationSingleton(const OGDBSimulationSingleton &) ; 				// declared but not implemented

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OGDBSimulationSingleton) ;
} ; 

_ACS_END_NAMESPACE


#endif // _OGDBSimulationSingleton_H_

