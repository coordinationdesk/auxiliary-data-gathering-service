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

	$Prod:$

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/01/25 12:16:43  marpas
	using pattern::Singleton from libException
	
	Revision 2.1  2013/01/24 10:34:27  marpas
	qa warnings
	
	Revision 2.0  2013/01/21 17:52:31  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.2  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
*/


#ifndef _DGDBSimulationSingleton_H_
#define _DGDBSimulationSingleton_H_ 

#include <acs_c++config.hpp>
#include <exStream.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


//
//
// class DGDBSimulationSingleton
//
//

class DGDBSimulationSingleton : public Singleton<DGDBSimulationSingleton > // PRQA S 2109, 2153
{
	friend DGDBSimulationSingleton* Singleton< DGDBSimulationSingleton >::instance(); // PRQA S 2107
public:
	virtual ~DGDBSimulationSingleton() throw() ; 

	bool simulation() const ; // return true if in simulation mode
	void simulation(bool) ; // return true if in simulation mode
	
protected:
	DGDBSimulationSingleton() ; 
	
private:
	DGDBSimulationSingleton &operator=(const DGDBSimulationSingleton &) ; 	// declared but not implemented
	DGDBSimulationSingleton (const DGDBSimulationSingleton &) ; 				// declared but not implemented

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DGDBSimulationSingleton);
} ; 


_ACS_END_NAMESPACE


#endif // _DGDBSimulationSingleton_H_

