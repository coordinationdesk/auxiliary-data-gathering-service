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
*/


#ifndef _TimeConverterSingleton_H_
#define _TimeConverterSingleton_H_ 

#include <acs_c++config.hpp>
#include <TimeConverter.h>
#include <exException.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

class TimeConverterSingleton // PRQA S 2109
{
public:
	/**
	 * \brief  Return the unique instance of this class. 
	 * \retval A pointer to the unique instance of TimeConverterSingleton   
	 */ 
	static TimeConverterSingleton* Instance();

	/**
	 * \brief  Class destructor 
	 *    
	 */ 
	virtual ~TimeConverterSingleton() ; 

	/**
	 * \brief  Register a singleton. Called by subclasses in order
	 *			to register themselves to the singleton registry.
	 *    
	 */
	static void Register(TimeConverterSingleton*);

	/**
	 * \brief  Return the Time converter object
	 *			
	 * \retval A pointer to the right instance of TimeConverter.   
	 */
	virtual TimeConverter* getConverter()=0;

protected:
	/**
	 * \brief  Default Class constructor 
	 *    
	 */
	TimeConverterSingleton() ;


private:
	TimeConverterSingleton &operator=(const TimeConverterSingleton &) ; 	// declared but not implemented
	TimeConverterSingleton (const TimeConverterSingleton &) ; 				// declared but not implemented

private:
	static TimeConverterSingleton*	_instance;
	static acs::ThreadSafe 			_mutex;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(TimeConverterSingleton) ;

} ; 


_ACS_END_NAMESPACE

#endif // _TimeConverterSingleton_H_

