// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 2.4  2017/05/18 12:19:12  davide.tiriticco
	Provided function computePolesTimesInInterval
	
	Revision 2.3  2017/05/18 08:21:40  davide.tiriticco
	General refactoring
	
	Revision 2.2  2017/05/16 09:22:11  davide.tiriticco
	Optimized and debugged getHaldOrbitID()
	
	Revision 2.1  2017/05/15 10:31:54  davide.tiriticco
	PoleInfo first add
	
*/

#ifndef POLEINFO_H_
#define POLEINFO_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <OrbitPropagator.h>
#include <AbsOrbitInfo.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

exDECLARE_EXCEPTION(exPoleInfoException, exException);

// FORWARD DECLARATION
class OrbitPropagator;

class PoleInfo {
public:
	typedef struct { bool north; long double time; } PoleTime;

	/**
	 * \brief	Class constructor 
	 * \param	const std::string& satName:	The satellite name
	 * \param	int mission: The mission number. 1 by default. 
	 */	
	explicit PoleInfo(const std::string& satName, int mission = 1);


	/**
	 * \brief	Class destructor 
	 *    
	 */
	virtual ~PoleInfo() throw();

	/**
	 * \brief	Class copy constructor 
	 */ 
	PoleInfo(const PoleInfo&); 

	/**
	 * \brief	Assignment operator overload
	 */
	PoleInfo &operator=(const PoleInfo&);
	
	/**
	 * \brief	Returns poles time associated to the half orbit id
	 * \param	long halfOrbitId: The half orbit id
	 * \return	Returns poles time associated to the half orbit id
	 */
	std::pair<PoleTime, PoleTime> polesTime(long halfOrbitId);
	
	/**
	 * \brief	Returns the half orbit id that contains the input time
	 * \param	long double time: The time used to retrieve the associated half orbit id.
	 * \return	Returns the half orbit id that includes the input time and that MUST be used to get the associated poles time.
	 */
	long computePolesTime(long double time);
	
	/**
	 * \brief	Compute all poles time of all halves orbits included in startTime and stopTime. Returns a vector containing all halves orbits ids.
	 * \param	long double startTime: The startTime for poles time computation
	 * \param	long double stopTime: The stopTime for poles time computation
	 */
	void computePolesTimesInInterval(long double startTime, long double stopTime, std::vector<long>& halvesOrbits);
			
	/**
	 * \brief	Convert the absolute orbit id in input to the associated halves orbits ids
	 * \param	long absOrbit: The absolute orbit to be converted
	 */
	static std::pair<long, long> absOrbitToHalfOrbit(long absOrbit);

	/**
	 * \brief	Convert the half orbit id in input to the associated absolute orbit id
	 * \param	long halfOrbit: The half orbit to be converted
	 */
	static long halfOrbitToAbsOrbit(long halfOrbit);
	
private:

	/**
	 * \brief	Returns the anxTime associated to the input time. If the true anomaly is larger than 270 degrees, 
	 * 			returns the anxTime of the next absolute orbit.
	 * \param	OrbitPropagator* propagator: The already initialized orbit propagator
	 * \param	long double time: The reference input time
	 */ 
	long double getCorrectAnxTime(OrbitPropagator* propagator, long double time);

	/**
	 * \brief	Applies the newton-raphson method to compute the pole estimation.
	 * \param	OrbitPropagator* propagator: The already initialized orbit propagator
	 * \param	ProcessingTime& time: The reference input time
	 * \param	long double deltaTime: The deltaTime used for derivative computation
	 * \param	long double deltaPhi: The expected offset in deg from the anx. (e.g.; 90 deg the for north pole, 270 deg for the south pole)
	 */
	long double newtonEstimation(OrbitPropagator* propagator, ProcessingTime& time, long double deltaTime, long double deltaPhi);

	/**
	 * \brief	Returns the time interval estimation of an half orbit that covers startTime. 
	 * \param	long double startTime: The startTime used as reference for computing north and south pole time (half orbit).
	 * \param	OrbitPropagator* propagator: Pointer to the orbit propagator to be used. If not provided this function will configure it's own propagator.
	 * \return	Returns a pair containing poles time in ascending order.
	 */ 
	std::pair<PoleTime, PoleTime> getPolesTime(long double startTime, OrbitPropagator* propagator = 0L);
	
	/**
	 * \brief	Default Constructor. Defined but not implemented.
	 */
	PoleInfo();
	
	/**
	 * \brief	Propagates to processing time pt and returns the angle with the ascending node.
	 * \param	const OrbitPropagator& propagator: The orbit propagator
	 * \param	ProcessingTime pt: The processing time for propagation
	 */
	long double propagAndGetTrueAnomaly(OrbitPropagator* propagator, ProcessingTime pt);
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PoleInfo);

	std::map<long, std::pair<PoleTime, PoleTime> > _halfOrbitsPoleTimes;
	std::string _satName;												/* The satellite name */
	int _mission;														/* Mission id */
	int _propagationModel;												/* Propagation model */
};

_ACS_END_NAMESPACE

#endif /* POLEINFO_H_ */
