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

#ifndef _KeplerianElement_H_
#define _KeplerianElement_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <ProcessingTime.h>
#include <AsciiTime.h>
#include <PropagFormat.h>

_ACS_BEGIN_NAMESPACE(acs)

	
class KeplerianElement 
{
public:


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	KeplerianElement();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	~KeplerianElement() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	KeplerianElement(const KeplerianElement & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	KeplerianElement &operator=(const KeplerianElement &);



	/**
	 * \brief  Set the Semi Major Axis field value
	 *    
	 */
	void semiMajorAxis(double a) { _semiMajorAxisNormalised = a; };

	/**
	 * \brief  Set the Eccentricity field value
	 *    
	 */
	void eccentricity(double e) { _eccentricity = e; };

	/**
	 * \brief  Set the Inclination field value
	 *    
	 */
	void inclination(double i) { _inclination = i; };

	/**
	 * \brief  Set the Right ascension of the ascending node field value
	 *    
	 */
	void rightAscentionANX(double ra) { _rightAscentionANX = ra; };

	/**
	 * \brief  Set the Argument of perigee field value
	 *    
	 */
	void argumentOfPerigee(double w) { _argumentOfPerigee = w; };

	/**
	 * \brief  Set the Mean anomaly field value
	 *    
	 */
	void meanAnomaly(double m) { _meanAnomaly = m; };

	/**
	 * \brief  Set the reference system field value
	 *    
	 */
	void refSystem(const PropagFormat::RefCoordinateFrames& ref) { _refSystem = ref; };


	/**
	 * \brief  Return the Semi Major Axis normalised field value
	 *    
	 */
	double semiMajorAxis() const throw() { return _semiMajorAxisNormalised; };

	/**
	 * \brief Return the Eccentricity field value
	 */
	double eccentricity() const throw() { return _eccentricity; };

	/**
	 * \brief  Return the Inclination field value
	 *    
	 */
	double inclination() const throw() { return _inclination;};

	/**
	 * \brief  Return the Right ascension of the ascending node field value
	 *    
	 */
	double rightAscentionANX() const throw() { return _rightAscentionANX; };

	/**
	 * \brief  Return the  Argument of perigee field value
	 */
	double argumentOfPerigee() const throw() { return _argumentOfPerigee; };

	/**
	 * \brief  Return the Mean anomaly field value
	 *    
	 */
	double meanAnomaly() const throw() { return _meanAnomaly;};

	/**
	 * \brief  Return the reference system field value
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

	ProcessingTime	procTime_ ;
	AsciiTime		asciiTime_ ;

	double		_semiMajorAxisNormalised;
	double		_eccentricity;
	double		_inclination;
	double		_rightAscentionANX;
	double		_argumentOfPerigee;
	double		_meanAnomaly;
	PropagFormat::RefCoordinateFrames	_refSystem;


};
_ACS_END_NAMESPACE

#endif //_KeplerianElement_H_
