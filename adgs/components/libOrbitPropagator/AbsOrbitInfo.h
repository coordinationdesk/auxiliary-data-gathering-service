// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

    $Log$ 
*/

#ifndef _AbsOrbitInfo_H_
#define _AbsOrbitInfo_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class AbsOrbitInfo 
{
public:


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	AbsOrbitInfo();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~AbsOrbitInfo() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	AbsOrbitInfo(const AbsOrbitInfo & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	AbsOrbitInfo &operator=(const AbsOrbitInfo &);

	/**
	 * \brief  Set the Absolute Orbit field value
	 *    
	 */
	void absOrbit(long int value) { _absOrbit = value; };

	/**
	 * \brief  Set the delta seconds since ascending node field value
	 *    
	 */
	void deltaSecANX(long int sec) { _deltaSecondsANX = sec; };

	/**
	 * \brief  Set microseconds within seconds
	 *    
	 */
	void microseconds(long int msec) { _microseconds = msec; };

	/**
	 * \brief  Return the Absolute Orbit field value
	 *    
	 */
	long int absOrbit() const { return _absOrbit; };

	/**
	 * \brief  Get the delta seconds since ascending node field value
	 */
	long int deltaSecANX() const { return _deltaSecondsANX; };

	/**
	 * \brief  Get microseconds within seconds
	 *    
	 */
	long int microseconds() const { return _microseconds;};

	/**
	 * \brief  Dump attributes values
	 *    
	 */
	std::string show() const;

private:

	long int 	_absOrbit;
	long int 	_deltaSecondsANX;
	long int 	_microseconds;


};
_ACS_END_NAMESPACE

#endif //_AbsOrbitInfo_H_
