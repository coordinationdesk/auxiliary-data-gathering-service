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
	Revision 1.7  2014/02/13 11:03:11  giucas
	New log macros adopted
	
	Revision 1.6  2012/04/18 08:43:43  giucas
	Compilation warnings removed
	
	Revision 1.5  2007/07/23 16:55:54  giucas
	propagation interval elongated by 0.123 days
	
	Revision 1.4  2007/07/20 13:41:16  giucas
	trying to get rid of a propagator bug dealing with anxs
	
	Revision 1.3  2007/07/19 12:42:41  marpas
	GOCE work in progress
	
	Revision 1.2  2007/05/14 16:09:43  giucas
	missing counter increment added in get method
	
	Revision 1.1  2007/05/14 12:40:31  giucas
	First issue
	

*/

#include <AnxInfo.h>
#include <OrbitPropagatorSingleton.h>
#include <StateVector.h>
#include <ExtraInfo.h>
#include <OrbitPropagator.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <sys/types.h> // PRQA S 1013
#include <unistd.h>
 
_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AnxInfo)

//Class constructors
AnxInfo::AnxInfo(const string& satelliteName, int mission) :
	_propagModel(int(PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL + PropagFormat::OP_PROPAG_MODEL_AUTO + PropagFormat::OP_PROPAG_MODEL_DOUBLE)), // PRQA S 3081
	_satelliteName(StringUtils::capitalize(satelliteName )),
	_mission(mission),
	_orb2Anx()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"AnxInfo::Id: " << Id )
} 

//Class destructor
AnxInfo::~AnxInfo() throw()
{
}

//copy constructor
AnxInfo::AnxInfo(const AnxInfo & e) :
	_propagModel(e._propagModel),
	_satelliteName(e._satelliteName),
	_mission(e._mission),
	_orb2Anx(e._orb2Anx)
{
}

//operator = 
AnxInfo&  AnxInfo::operator=(const AnxInfo & e)
{
	if (this != &e) 
	{
		_propagModel	= e._propagModel;
		_satelliteName	= e._satelliteName ;
		_mission		= e._mission ;
		_orb2Anx		= e._orb2Anx ;
	}
	return *this ;
}

void AnxInfo::get(double start, double stop, vector<double>& result, vector<int> &orbits)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for interval [ " << setprecision(17) << start << " , " << stop << " ]" )

	OrbitPropagator* propagator = (OrbitPropagatorSingleton::Instance())->getPropagator();
	propagator->setSatelliteID(_satelliteName,_mission);
	double elongStop = stop+0.123 ;
	propagator->initPropagFromFile(PropagFormat::OP_TIMEMOD_FOS_PREDICTED, start, elongStop, _propagModel);
	
	double propagTime = start ;
	
	StateVector stVec;
    ProcessingTime pt(propagTime,TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC) ;
	
	_orb2Anx.clear() ;
    double anxUtcTime = 0 ;
    double nodalPeriod = 0 ; 
	do { // PRQA S 4234, 4236
        pt.setValue(propagTime) ;
        stVec.setProcessingTime(pt) ;
		propagator->propag(stVec);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Propagation at time " << setprecision(17) << propagTime << " executed... "<< endl << stVec.show() )

		ExtraInfo extraInf;
		propagator->propagExtraInfo(extraInf);
		vector<double> modelDep = extraInf.modelDependent();
		anxUtcTime = modelDep[PropagFormat::OP_PROPAG_EXTRA_DEP_UTC_CURRENT_ANX];
		nodalPeriod = modelDep[PropagFormat::OP_PROPAG_EXTRA_DEP_NODAL_PERIOD];
		int absoluteOrbit =	static_cast<int>(modelDep[PropagFormat::OP_PROPAG_EXTRA_DEP_ORBIT_NUMBER]) ; // PRQA S 3081
		nodalPeriod/=86400.0; // PRQA S 4400

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"get: " << setprecision(17) << anxUtcTime << " returned with propagTime: " << propagTime )

		if( anxUtcTime > propagTime )
		{
			if (anxUtcTime <= stop) { // but last - see while condition incremented by a nodalPeriod
				result.push_back(anxUtcTime - nodalPeriod);
				orbits.push_back(absoluteOrbit-1) ;
			}
			_orb2Anx[absoluteOrbit-1] = anxUtcTime - nodalPeriod ;
		}
		else {
			if (anxUtcTime <= stop) { // but last - see while condition incremented by a nodalPeriod
				result.push_back(anxUtcTime);
				orbits.push_back(absoluteOrbit) ;
			}
			_orb2Anx[absoluteOrbit] = anxUtcTime ;
		}

		//point to the next anx time + 1 second
		propagTime = anxUtcTime + nodalPeriod + 0.00001157 ; // PRQA S 4400
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Next Propagation at time " << setprecision(17) << propagTime )
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"nodalPeriod " << setprecision(17) << nodalPeriod << " stop+nod " << stop + nodalPeriod )
	} while( anxUtcTime <= stop + nodalPeriod ) ;

}

double AnxInfo::anx(int orbit)
{
	map<int, double>::iterator cur = _orb2Anx.find(orbit) ;
	if (cur == _orb2Anx.end()) {
		ostringstream os ;
		os << orbit << " not found in propagated set or no propagation yet." ;
		ACS_THROW(exIllegalValueException(os.str())) ; // PRQA S 3081
	}

	return cur->second ;
}
_ACS_END_NAMESPACE
