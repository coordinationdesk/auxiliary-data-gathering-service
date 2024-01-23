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
	Revision 2.10  2017/11/15 15:55:56  davide.tiriticco
	Fixed bug on abs orbit id computation when executed close to the anx node
	
	Revision 2.9  2017/05/23 11:58:29  davide.tiriticco
	Removed debug code
	
	Revision 2.8  2017/05/22 11:49:38  davide.tiriticco
	Fixed several bugs
	
	Revision 2.7  2017/05/18 12:33:51  davide.tiriticco
	compute
	
	Revision 2.6  2017/05/18 12:19:12  davide.tiriticco
	Provided function computePolesTimesInInterval
	
	Revision 2.5  2017/05/18 08:21:40  davide.tiriticco
	General refactoring
	
	Revision 2.4  2017/05/16 09:22:11  davide.tiriticco
	Optimized and debugged getHaldOrbitID()
	
	Revision 2.3  2017/05/15 11:44:39  davide.tiriticco
	Applied best practices
	
	Revision 2.2  2017/05/15 11:10:18  davide.tiriticco
	Fixed bug for trueAnomalies larger than 270 degress
	
	Revision 2.1  2017/05/15 10:31:54  davide.tiriticco
	PoleInfo first add
	
*/

#include <PoleInfo.h>
#include <OrbitPropagatorSingleton.h>
#include <StateVector.h>
#include <ExtraInfo.h>
#include <OrbitPropagator.h>
#include <Filterables.h>
#include <cmath>
#include <cstdlib>

#define JD_1SEC						0.000001157
#define ANX_TIME_TOLERANCE			(300.0 / 86400.0)
#define NORTH_POLE_ANGLE			90.0
#define SOUTH_POLE_ANGLE			270.0

#define VALIDITY_TIME_INTERVAL		(6600.0 / 86400.0)		// 110 minutes
#define FLOOR_DSEC(JDTIME) 			(floor((JDTIME) * 86400.0 * 10.0) / (86400.0 * 10.0))

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(PoleInfo);

/*
 * Class constructor 
 */	
PoleInfo::PoleInfo(const std::string& satName, int mission) :
	_halfOrbitsPoleTimes(),
	_satName(satName), 
	_mission(mission),
	_propagationModel(int(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL + PropagFormat::OP_PROPAG_MODEL_AUTO + PropagFormat::OP_PROPAG_MODEL_DOUBLE)) // PRQA S 3081
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"PoleInfo::Id: " << Id );
}

/*
 * Class destructor 
 */
PoleInfo::~PoleInfo() throw() { }

/*
 * Class copy constructor 
 */ 
PoleInfo::PoleInfo(const PoleInfo& info) :
	_halfOrbitsPoleTimes(),
	_satName(), 
	_mission(),
	_propagationModel(int(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL + PropagFormat::OP_PROPAG_MODEL_AUTO + PropagFormat::OP_PROPAG_MODEL_DOUBLE))
{
	*this = info;
}

/*
 * Assignment operator overload
 */
PoleInfo& PoleInfo::operator=(const PoleInfo& info) {
	if (this != &info) {
		_halfOrbitsPoleTimes = info._halfOrbitsPoleTimes;
		_satName = info._satName;
		_mission = info._mission;
		_propagationModel = info._propagationModel;
	}
	
	return *this;
}

/*
 * Converts the absolute orbit id in input to the associated halves orbits ids
 */
std::pair<long, long> PoleInfo::absOrbitToHalfOrbit(long absOrbit) {
	return std::pair<long, long>(2 * absOrbit, 2 * absOrbit + 1);
}

/*
 * Converts the half orbit id in input to the associated absolute orbit id
 */
long PoleInfo::halfOrbitToAbsOrbit(long halfOrbit) {
	return (halfOrbit - halfOrbit % 2) / 2;
}

/*
 * Propagates to processing time pt and returns the latitude argument with the ascending node.
 */
long double PoleInfo::propagAndGetTrueAnomaly(OrbitPropagator* propagator, ProcessingTime pt) {
	StateVector sv;
	ExtraInfo extraInf;
	
	try {
		sv.setProcessingTime(pt);
		propagator->propag(sv);
		propagator->propagExtraInfo(extraInf);
	} catch(std::exception& e) {
		ACS_THROW(exPoleInfoException(e.what()));
	}
	
	return extraInf.modelIndependent()[PropagFormat::OP_PROPAG_EXTRA_OSC_TRUE_LAT];
}

/*
 * Returns the anxTime associated to the input time. If the true anomaly is larger than 270 degrees, 
 * returns the anxTime of the next absolute orbit.
 */
long double PoleInfo::getCorrectAnxTime(OrbitPropagator* propagator, long double time) {
	ExtraInfo extraInf;
	StateVector sv;
	ProcessingTime pt(time, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC);
	
	long double trueAnomaly = propagAndGetTrueAnomaly(propagator, pt);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "True anomaly: " << trueAnomaly << ".");
	
	propagator->propagExtraInfo(extraInf);
	
	long double nodalPeriod = extraInf.modelDependent()[PropagFormat::OP_PROPAG_EXTRA_DEP_NODAL_PERIOD] / 86400.0;
	long double anxTime = extraInf.modelDependent()[PropagFormat::OP_PROPAG_EXTRA_DEP_UTC_CURRENT_ANX];
	if (trueAnomaly >= SOUTH_POLE_ANGLE) {
		anxTime += nodalPeriod + JD_1SEC;
		pt.setValue(anxTime);
		sv.setProcessingTime(pt);
		
		// Can't be avoided for correct anxTime value
		propagator->propag(sv);
		propagator->propagExtraInfo(extraInf);
		anxTime = extraInf.modelDependent()[PropagFormat::OP_PROPAG_EXTRA_DEP_UTC_CURRENT_ANX];
	}
	
	return anxTime;
}

/*
 * Compute all poles time of all halves orbits included in startTime and stopTime. Returns a vector containing all halves orbits ids.
 */
void PoleInfo::computePolesTimesInInterval(long double startTime, long double stopTime, std::vector<long>& halvesOrbits) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called with input time interval [ " << std::setprecision(17) << startTime << ", " << stopTime << "].");
	halvesOrbits.clear();
	
	StateVector sv;
	AbsOrbitInfo startOrbitInfo, stopOrbitInfo;
	OrbitPropagator* propagator = 0L;
	
	double initStartTime(startTime - VALIDITY_TIME_INTERVAL);
	double initStopTime(stopTime + VALIDITY_TIME_INTERVAL);
	
	// Init propagator
	propagator = (OrbitPropagatorSingleton::Instance())->getPropagator();
	propagator->setSatelliteID(_satName, _mission);
	propagator->initPropagFromFile(PropagFormat::OP_TIMEMOD_FOS_PREDICTED, initStartTime, initStopTime, _propagationModel);

	// Propagate and get the anx times
	long double startAnxTime = 0.0, stopAnxTime = 0.0;
	startAnxTime = getCorrectAnxTime(propagator, startTime);
	if (startTime == stopTime)  {
		stopAnxTime = startAnxTime;
	} else {
		stopAnxTime = getCorrectAnxTime(propagator, stopTime);
	}
	
	ExtraInfo extraInf;
	propagator->propagExtraInfo(extraInf);
	long double nodalPeriod = extraInf.modelDependent()[PropagFormat::OP_PROPAG_EXTRA_DEP_NODAL_PERIOD] / 86400.0;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"retrieved extra info ["
								<< ", startAnxTime = " << std::setprecision(17) << startAnxTime 
								<< ", stopAnxTime = " << std::setprecision(17) << stopAnxTime 
								<< "].");
	
//	ProcessingTime ptStart(startAnxTime + JD_1SEC, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC);
//	ProcessingTime ptStop(stopAnxTime + JD_1SEC, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC);
	ProcessingTime ptStart(startAnxTime + ANX_TIME_TOLERANCE, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC);
	ProcessingTime ptStop(stopAnxTime + ANX_TIME_TOLERANCE, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC);

	propagator->timeToAbsOrbit(ptStart, startOrbitInfo);
	propagator->timeToAbsOrbit(ptStop, stopOrbitInfo);	
	long startAbsOrbit = startOrbitInfo.absOrbit();
	long stopAbsOrbit = startOrbitInfo.absOrbit();

	long halfStartOrbit = absOrbitToHalfOrbit(startOrbitInfo.absOrbit()).first;
	long halfStopOrbit = absOrbitToHalfOrbit(stopOrbitInfo.absOrbit()).second;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start ANX time absolute orbit is " << startAbsOrbit << ".");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop ANX time absolute orbit is " << stopAbsOrbit << ".");

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting pole time computation from orbit " << halfStartOrbit << " to orbit " << halfStopOrbit << ".");
	
	long double currAnxTime = startAnxTime;
	for (long currentOrbit = halfStartOrbit; currentOrbit <= halfStopOrbit; currentOrbit += 2) {											
		std::map<long, std::pair<PoleTime, PoleTime> >::iterator it;
		
		// Check if the first half orbit times are already in cache, if not compute them
		if ((it = _halfOrbitsPoleTimes.find(currentOrbit)) == _halfOrbitsPoleTimes.end()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Computing poles time for half orbit " << currentOrbit);
			_halfOrbitsPoleTimes[currentOrbit] = getPolesTime(currAnxTime + JD_1SEC, propagator);
		}
		
		// Check if the second half orbit times are already in cache, if not compute them
		if ((it = _halfOrbitsPoleTimes.find(currentOrbit + 1)) == _halfOrbitsPoleTimes.end()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Computing poles time for half orbit " << currentOrbit + 1);
			_halfOrbitsPoleTimes[currentOrbit + 1] = getPolesTime(currAnxTime + nodalPeriod / 2.0 + JD_1SEC, propagator);
		}
		
		halvesOrbits.push_back(currentOrbit);
		halvesOrbits.push_back(currentOrbit + 1);
		currAnxTime = currAnxTime + nodalPeriod;
	}

	// Now the cache is populated, get start and stop orbit times
	std::pair<PoleTime, PoleTime> startPoleTime = _halfOrbitsPoleTimes[halfStartOrbit];
	std::pair<PoleTime, PoleTime> stopPoleTime = _halfOrbitsPoleTimes[halfStopOrbit];
	
	// If start time is not included in the first computed orbit times, remove it from the vector
	if (startTime < startPoleTime.first.time || startTime >= startPoleTime.second.time) {
		halvesOrbits.erase(halvesOrbits.begin());
	}

	// If stop time is not included in the last computed orbit times, remove it from the vector
	if (stopTime < stopPoleTime.first.time || stopTime >= stopPoleTime.second.time) {
		halvesOrbits.erase(halvesOrbits.begin() + halvesOrbits.size() - 1);
	}
}

/*
 * Returns the half orbit id that contains the input time
 */
long PoleInfo::computePolesTime(long double time) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called with input time [ " << std::setprecision(17) << time << "].");
	
	std::vector<long> halvesOrbits;
	try {
		computePolesTimesInInterval(time, time, halvesOrbits);
	} catch (exPoleInfoException& e) {
		ACS_THROW(exPoleInfoException(e.what()));
	}
	return halvesOrbits[0];
}

/*
 * Returns poles time associated to the half orbit id
 */
std::pair<PoleInfo::PoleTime, PoleInfo::PoleTime> PoleInfo::polesTime(long halfOrbitID) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB," called for orbit id [ " << halfOrbitID << "].");
	
	std::map<long, std::pair<PoleInfo::PoleTime, PoleInfo::PoleTime> >::iterator cur = _halfOrbitsPoleTimes.find(halfOrbitID);
	if (cur == _halfOrbitsPoleTimes.end()) {
		std::ostringstream os ;
		os << " poles time for halfOrbitID " << halfOrbitID << " can't be found." ;
		ACS_THROW(exIllegalValueException(os.str())) ; // PRQA S 3081
	}
	
	return cur->second;
}

/*
 * Applies the newton-raphson method to compute the pole estimation.
 */
long double PoleInfo::newtonEstimation(OrbitPropagator* propagator, ProcessingTime& pt, long double deltaTime, long double deltaPhi) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Invoked on start time " << std::setprecision(17) << pt.getValue() << ".");

		long double trueAnomaly = 0.0;
		long double estimation = pt.getValue();
		long double lastEstimation = estimation;
		long double leftTrueAnomaly = 0.0;
		long double rightTrueAnomaly = 0.0;
		
		int depth = 20;
		do {
			// Propagate to the current estimation and get true anomaly
			pt.setValue(estimation);
			trueAnomaly = propagAndGetTrueAnomaly(propagator, pt);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"true anomaly at time t [" << std::setprecision(17) << trueAnomaly << "].");

			// propagate left and get true anomaly
			pt.setValue(estimation - deltaTime);
			leftTrueAnomaly = propagAndGetTrueAnomaly(propagator, pt);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"true anomaly at time t - dt [" << std::setprecision(17) << leftTrueAnomaly << "].");

			// propagate right and get true anomaly
			pt.setValue(estimation + deltaTime);
			rightTrueAnomaly = propagAndGetTrueAnomaly(propagator, pt);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"true anomaly at time t + dt [" << std::setprecision(17) << rightTrueAnomaly << "].");

			// apply newton-raphson tangents method (x_n+1 = x_n - f(x_n)/f'(x_n))
			lastEstimation = estimation;
			estimation = estimation - (trueAnomaly - deltaPhi) / ((rightTrueAnomaly - leftTrueAnomaly) / (2.0 * deltaTime));

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"true anomaly at time t [" << std::setprecision(17) << trueAnomaly << "].");
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Fixing pole time value using [" << std::fixed << std::setprecision(17) << std::abs(estimation - lastEstimation) * 86400.0 << "] seconds.");
		} while ((std::abs(estimation - lastEstimation) > JD_1SEC) && --depth);
		
		return estimation;
}

/*
 * Returns the time interval of the half orbit [north-pole, south-pole] that covers startTime.
 */  
std::pair<PoleInfo::PoleTime, PoleInfo::PoleTime> PoleInfo::getPolesTime(long double startTime, OrbitPropagator* propagator) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for startTime [ " << std::setprecision(17) << startTime << "].");

	long double anxUtcTime = 0.0;
	long double nodalPeriod = 0.0;
	long double deltaTime = 0.00001;
	long double northEstimation = 0.0;
	long double southEstimation = 0.0;
	long double propagationTime = startTime;
		
	// Initialize propagator
	if (0L == propagator) {
		double initStartTime(startTime - VALIDITY_TIME_INTERVAL);
		double initStopTime(startTime + VALIDITY_TIME_INTERVAL);
	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"initializing propagator in interval [ " << std::setprecision(17) << initStartTime << ", " << std::setprecision(17) << initStopTime << "].");
	
		propagator = (OrbitPropagatorSingleton::Instance())->getPropagator();
		propagator->setSatelliteID(_satName, _mission);
		propagator->initPropagFromFile(PropagFormat::OP_TIMEMOD_FOS_PREDICTED, initStartTime, initStopTime, _propagationModel);
	}

	StateVector sv;
	ProcessingTime pt(propagationTime,TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC);
			
	// Propagate to startTime
	sv.setProcessingTime(pt);
	propagator->propag(sv);
	
	// Get extra info
	ExtraInfo extraInf;
	propagator->propagExtraInfo(extraInf);
	std::vector<double> modelDep = extraInf.modelDependent();
	
	// Get anx time, nodal period and absolute orbit
	anxUtcTime = modelDep[PropagFormat::OP_PROPAG_EXTRA_DEP_UTC_CURRENT_ANX];
	nodalPeriod = modelDep[PropagFormat::OP_PROPAG_EXTRA_DEP_NODAL_PERIOD] / 86400.0;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"retrieved anx time [" << std::setprecision(17) << anxUtcTime << "], and nodal period [" << std::setprecision(17) << nodalPeriod << "].");
	
	northEstimation = anxUtcTime + nodalPeriod / 4.0;
	southEstimation = anxUtcTime - nodalPeriod / 4.0;
	double trueAnomaly = propagAndGetTrueAnomaly(propagator, pt);

	// If true anomaly larger than 90 degrees, switch to the next south pole time estimation
	if (trueAnomaly >= NORTH_POLE_ANGLE) {
		southEstimation += nodalPeriod;
	}
	
	// If true anomaly larger than 270 degrees, switch to the next north pole time estimation
	if (trueAnomaly >= SOUTH_POLE_ANGLE) {
		northEstimation += nodalPeriod;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"True anomaly for the input time [ " << std::setprecision(17) << trueAnomaly << "].");		

	// Estimate north pole time 
	pt.setValue(northEstimation);
	northEstimation = newtonEstimation(propagator, pt, deltaTime, NORTH_POLE_ANGLE);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Estimated north pole time [" << std::setprecision(17) << northEstimation << "].");

	// Estimate south pole
	pt.setValue(southEstimation);
	southEstimation = newtonEstimation(propagator, pt, deltaTime, SOUTH_POLE_ANGLE);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Estimated south pole time [" << std::setprecision(17) << southEstimation << "].");

	// Floor north and south pole times for dsec accuracy
	std::pair<PoleInfo::PoleTime, PoleInfo::PoleTime> polesTime;
	polesTime.first.time = FLOOR_DSEC(northEstimation);
	polesTime.second.time = FLOOR_DSEC(southEstimation);
	
	polesTime.first.north = true;
	polesTime.second.north = false;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Computed north-pole time [" << std::fixed << std::setprecision(17) << polesTime.first.time << "] and south-pole time [" << std::fixed << std::setprecision(17) << polesTime.second.time << "] seconds.");
	
	// Sort the pair before returning it
	if (polesTime.first.time > polesTime.second.time) {
		PoleInfo::PoleTime tmp = polesTime.first;
		polesTime.first = polesTime.second;
		polesTime.second = tmp;
	}
	
	return polesTime;
}

_ACS_END_NAMESPACE

