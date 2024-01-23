// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2013, Advanced Computer Systems , Inc.
 * 	Via Della Bufalotta, 378 - 00139 Roma - Italy
 * 	http://www.acsys.it
 * 
 * 	All Rights Reserved.
 * 
 * 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 * 	the contents of this file may not be disclosed to third parties, copied or
 * 	duplicated in any form, in whole or in part, without the prior written
 * 	permission of Advanced Computer Systems, Inc.
 * 
 * 	$Prod:  $
 * 
 * 	$Id$
 * 
 * 	$Author$
 * 
 */



#ifndef _ACSTimeConvSingleton_H_
#define _ACSTimeConvSingleton_H_ 

#include <acs_c++config.hpp>
#include <TimeConverterSingleton.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)
class ACSTimeConvWrap ;

//
//
// class ACSTimeConvSingleton
//
//
#warning MP: use Singleton template
class ACSTimeConvSingleton : public TimeConverterSingleton // PRQA S 2109
{
public:
	/**
	 * \brief  Default Class constructor 
	 *    
	 */
	ACSTimeConvSingleton() ;


	/**
	 * \brief  Class destructor 
	 *    
	 */ 
	virtual ~ACSTimeConvSingleton() throw() ; 

	/**
	 * \brief  Return the Time converter object
	 *			
	 * \retval A pointer to the right instance of TimeConverter.   
	 */
	virtual TimeConverter* getConverter();

protected:



private:
	ACSTimeConvSingleton &operator=(const ACSTimeConvSingleton &) ;  // not implemented
	ACSTimeConvSingleton (const ACSTimeConvSingleton &) ; 	 // not implemented			

private:
	static ACSTimeConvWrap*	 _converter;
	acs::ThreadSafe		 	 _mutex;		
} ; 


_ACS_END_NAMESPACE


#endif // _ACSTimeConvSingleton_H_

