// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 2.1  2014/02/13 11:03:12  giucas
	New log macros adopted
	
	Revision 2.0  2006/02/28 09:40:42  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/04/21 10:50:09  giucas
	Removed dependencies from ACE. Using ThreadSafe class instead.
	

*/


#ifndef _OrbitPropagatorSingleton_H_
#define _OrbitPropagatorSingleton_H_ 

#include <acs_c++config.hpp>
#include <OrbitPropagator.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class OrbitPropagatorSingleton // PRQA S 2109
{
public:
	/**
	 * \brief  Return the unique instance of this class. 
	 * \retval A pointer to the unique instance of OrbitPropagatorSingleton   
	 */ 
	static OrbitPropagatorSingleton* Instance();

	/**
	 * \brief  Class destructor 
	 *    
	 */ 
	virtual ~OrbitPropagatorSingleton() ; 

	/**
	 * \brief  Register a singleton. Called by subclasses in order
	 *			to register themselves to the singleton registry.
	 *    
	 */
	static void Register(OrbitPropagatorSingleton*);

	/**
	 * \brief  Return the Orbit Propagator object
	 *			
	 * \retval A pointer to the right instance of OrbitPropagator.   
	 */
	virtual OrbitPropagator* getPropagator()=0;

protected:
	/**
	 * \brief  Default Class constructor 
	 *    
	 */
	OrbitPropagatorSingleton() ;


private:
	OrbitPropagatorSingleton &operator=(const OrbitPropagatorSingleton &) ; 	// declared but not implemented
	OrbitPropagatorSingleton (const OrbitPropagatorSingleton &) ; 				// declared but not implemented

private:
	static OrbitPropagatorSingleton*	_instance;
	static acs::ThreadSafe 				_mutex;


} ; 


_ACS_END_NAMESPACE


#endif // _OrbitPropagatorSingleton_H_

