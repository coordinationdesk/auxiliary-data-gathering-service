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

#ifndef _StateVector_H_
#define _StateVector_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <ProcessingTime.h>
#include <AsciiTime.h>
#include <PropagFormat.h>

_ACS_BEGIN_NAMESPACE(acs)

	
class StateVector 
{
public:


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	StateVector();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~StateVector() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	StateVector(const StateVector & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	StateVector &operator=(const StateVector &);



	/**
	 * \brief  Set the Osculationg Position vector field value
	 *    
	 */
	void position(const std::vector<double>& pos) { _pos = pos; };

	/**
	 * \brief  Set the Osculating Velocity vector field value
	 *    
	 */
	void velocity(const std::vector<double>& vel) { _vel = vel; };

	/**
	 * \brief  Set the Osculating Acceleration vector field value
	 *    
	 */
	void acceleration(const std::vector<double>& acc) { _acc = acc; };

	/**
	 * \brief  Set the reference system field value
	 *    
	 */
	void refSystem(PropagFormat::RefCoordinateFrames ref) throw() { _refSystem = ref; };


	/**
	 * \brief  Return the Osculationg Position vector field value
	 *    
	 */
	const std::vector<double> &position() const throw() { return _pos; };

	/**
	 * \brief  Get the Osculating Velocity vector field value
	 */
	const std::vector<double> &velocity() const throw() { return _vel; };

	/**
	 * \brief  Get the Osculating Acceleration vector field value
	 *    
	 */
	const std::vector<double> &acceleration() const throw() { return _acc;};

	/**
	 * \brief  Get the reference system field value
	 *    
	 */
	PropagFormat::RefCoordinateFrames refSystem() const throw() { return _refSystem;};

    const AsciiTime &getAsciiTime() const throw() { return asciiTime_ ; }
    void setAsciiTime(const AsciiTime &v) throw() { asciiTime_ = v ; }

    const ProcessingTime &getProcessingTime() const throw() { return procTime_ ; }
    void setProcessingTime(const ProcessingTime &v) throw() { procTime_ = v ; }
    void setProcessingTimeValue(long double v) throw() { procTime_.setValue(v) ; }
    void setProcessingTimeRef(TimeFormat::TimeReference r) throw() { procTime_.setRef(r) ; }
    void setProcessingTimeFormat(TimeFormat::ProcessingTimeFormat f) throw() { procTime_.setFormat(f) ; }
	/**
	 * \brief  Dump attributes values
	 *    
	 */
	std::string show() const;


private:

	ProcessingTime	procTime_;
	AsciiTime		asciiTime_;

	std::vector<double> _pos;
	std::vector<double> _vel;
	std::vector<double> _acc;
	PropagFormat::RefCoordinateFrames _refSystem;


};
_ACS_END_NAMESPACE

#endif //_StateVector_H_
