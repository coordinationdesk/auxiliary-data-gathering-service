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

#ifndef _RelOrbitInfo_H_
#define _RelOrbitInfo_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class RelOrbitInfo 
{
public:


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	RelOrbitInfo();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~RelOrbitInfo() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	RelOrbitInfo(const RelOrbitInfo & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	RelOrbitInfo &operator=(const RelOrbitInfo &);

	/**
	 * \brief  Set the Relative Orbit field value
	 *    
	 */
	void relOrbit(long int value) { _relOrbit = value; };

	/**
	 * \brief  Set the Cycle field value
	 *    
	 */
	void cycle(long int c) { _cycle = c; };

	/**
	 * \brief  Set the Phase field value
	 *    
	 */
	void phase(long int ph) { _phase = ph; };

	/**
	 * \brief  Set the Extra Info field value
	 *    
	 */
	void extraInfo(const std::vector<double>& info) { _extraInfo = info; };

	/**
	 * \brief  Return the Relative Orbit field value
	 *    
	 */
	long int relOrbit() const { return _relOrbit; };

	/**
	 * \brief  Get the Cycle field value
	 */
	long int cycle() const { return _cycle; };

	/**
	 * \brief  Get the Phase field value
	 *    
	 */
	long int phase() const { return _phase;};

	/**
	 * \brief  Return the Extra Info field value
	 *    
	 */
	std::vector<double> extraInfo() const { return _extraInfo; };

	/**
	 * \brief  Dump attributes values
	 *    
	 */
	std::string show() const;


private:

	long int 			_relOrbit;
	long int 			_cycle;
	long int 			_phase;
	std::vector<double>	_extraInfo;


};
_ACS_END_NAMESPACE

#endif //_RelOrbitInfo_H_
