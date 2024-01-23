// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: 

	$Id$

	$Author$

	$Log$
	Revision 5.2.2.2  2016/10/26 14:27:19  lucio.pulvirenti
	OPMAN-217: If exception got, exception is notified in debug mode only. A warning is issued with exception description (what() method).
	
	Revision 5.2.2.1  2016/10/26 12:06:26  lucio.pulvirenti
	OPMAN-217: iterate method rewritten since uncorrect. initPropagator and getGeoPointInfo private methods added. ACS_TEST_GEO_POINT_ITERATOR compilation flag
	defined to test iterate method w.r.t of time computing (neither propagation nor geo points computed).
	
	Revision 5.2  2014/03/27 12:48:15  marpas
	reflect changes in orbit propagation libraries family
	
	Revision 5.1  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.9  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.8  2012/03/06 15:54:04  marpas
	messages improved and standardized
	
	Revision 2.7  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.6  2009/03/06 11:55:48  marpas
	local headers included with double quotes
	
	Revision 2.5  2008/08/27 16:50:02  giucas
	Mission parameter added to default 1
	
	Revision 2.4  2007/01/10 11:37:03  giucas
	Updated OrbitPropagator lib calls for IF changes (CFI 3.5 porting)
	
	Revision 2.3  2006/07/05 08:46:10  giucas
	Old algo reintroduced
	
	Revision 2.2  2006/06/02 13:10:36  giucas
	Introduced automatic delta tuning to cover error cases arised during GOCE V2 DR (the delta was too large)
	
	Revision 2.1  2006/03/07 14:35:56  giucas
	Removed catch(...) in iterate method
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/12/05 11:12:59  giucas
	Added delta in start and stop initialization
	
	Revision 1.11  2005/12/02 16:23:57  giucas
	Added pid parameter to getValidationInterval call
	
	Revision 1.10  2005/09/13 10:03:49  giucas
	Suppressed exception throw in case of CFI initialization failure computing geo points
	
	Revision 1.9  2005/09/01 06:54:46  giucas
	Propagator initialization reset to default MEAN_KEPL
	
	Revision 1.8  2005/08/29 15:29:18  giucas
	Propagator initialized as MEAN_KEPL+AUTO+DOUBLE
	
	Revision 1.7  2005/08/29 11:49:29  giucas
	Propagator initialized with DOUBLE PropagationModel
	
	Revision 1.6  2005/05/13 15:06:34  giucas
	Improved debug message in iterate method
	
	Revision 1.5  2005/04/21 12:06:39  giucas
	Minor modifications
	
	Revision 1.4  2005/04/20 10:09:02  giucas
	Updated ExtraInfo vector indexes for lat and long in iterate method
	
	Revision 1.3  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.2  2004/11/08 16:14:02  giucas
	Added debug informations
	
	Revision 1.1  2004/11/04 15:37:34  giucas
	Imported source
	

*/

#include <GeoPointIterator.h>

#ifdef ACS_TEST_GEO_POINT_ITERATOR

#warning "ACS_TEST_GEO_POINT_ITERATOR defined"

#else

#include <OrbitPropagator.h>
#include <OrbitPropagatorSingleton.h>
	
#endif

#include <Filterables.h>
#include <sys/types.h>  // PRQA S 1013
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GeoPointIterator)


//Class constructor
#ifdef ACS_TEST_GEO_POINT_ITERATOR
GeoPointIterator::GeoPointIterator(const string& satName, int mission)
#else
GeoPointIterator::GeoPointIterator(const string& satName, int mission) :
    _propagator(OrbitPropagatorSingleton::Instance()->getPropagator()) 
#endif

{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id << " - Using satellite name " << satName) ;

#ifndef ACS_TEST_GEO_POINT_ITERATOR
	//configure OrbitPropagator	
	_propagator->setSatelliteID(satName,mission);
#endif	

}


//Class destructor
GeoPointIterator::~GeoPointIterator() throw() {}

vector<Metadata::GeoPointInfo>
GeoPointIterator::iterate(long double inStart, long double inStop, long double step, long double inDelta) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	
	const long double start = inStart;
	const long double stop = inStop;
	//express step in days
	const long double itStep = step/86400.0;
	const long double delta = inDelta;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "StartTime = " <<setprecision(12) << start // PRQA S 4400 2
 			<< " - StopTime = " << stop << " - Step = " << step << " (secs) [" << itStep << " dd] - Delta initialization interval = " << delta) ;

	vector<Metadata::GeoPointInfo> geoPoints;
	try {
#ifndef ACS_TEST_GEO_POINT_ITERATOR
		StateVector vec;
		vec.setProcessingTimeFormat(TimeFormat::TC_PROC);
		vec.setProcessingTimeRef(TimeFormat::TC_TIME_UTC);
#endif
		long double currentStart = start;
		long double currentStop = currentStart + delta;
		bool exit = false;
		if( inStart == inStop )	{
			ACS_LOG_WARNING("Start and stop time are equal: " <<setprecision(12) << inStart ) ;
			exit = true;
		}	

		while( not exit ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "currentStart = " <<setprecision(12) << currentStart // PRQA S 4400
 				<< " - currentStop = " << currentStop << " - Expected StopTime is " << stop) ;

			if( currentStart > stop )
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "currentStart (" <<setprecision(12) << currentStart // PRQA S 4400
 						<< ") >= Expected StopTime (" << stop << ") -> Exiting...") ;
				break;
			}

			if( currentStop > stop )
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "currentStop[" <<setprecision(12) << currentStop // PRQA S 4400
 						<< "] > Expected StopTime[" << stop << "] -> Propagate and exit...") ;
				currentStop = stop;
				exit = true;
			}

#ifdef ACS_TEST_GEO_POINT_ITERATOR
			initPropagatorStub(currentStart, currentStop, delta);
#else
			initPropagator(currentStart, currentStop, delta);
#endif

			//propagate from currentStart to currentStop
			bool currentStartSetToStop = false;
			while(currentStart <= currentStop) // PRQA S 4234, 4236
			{
				try {	
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to get geo coords for time " << setprecision(17) << currentStart);
#ifdef ACS_TEST_GEO_POINT_ITERATOR
					Metadata::GeoPointInfo point = getGeoPointInfoStub( currentStart );
#else
					Metadata::GeoPointInfo point = getGeoPointInfo( vec, currentStart );
#endif
					geoPoints.push_back(point);
				}
				catch(exception& ex)
				{
					ostringstream exMsg;
				   	exMsg << "There's a propag problem with time "  << setprecision(17) << currentStart ;
					ACS_THROW( exIllegalValueException( ex, exMsg.str() ) );		 // PRQA S 4400 2
				}

				currentStart += itStep;
				long double residual = stop - currentStart;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Current start just set to " << setprecision(17) << currentStart << ", current stop=" << currentStop 
										<< " stop=" << stop << "; (stop - current start)=" << residual);
				if( residual < 0.5*itStep && not currentStartSetToStop) {
					currentStop = stop;
					currentStart = currentStop;
					currentStartSetToStop = true;
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current start set to stop: " << setprecision(17) << currentStart);
				}
			}
			currentStop = currentStart + delta;

		} // while( not exit )
	} // try block
	catch(exception& e)
	{
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_NOTIFY_EX(e);
		ACS_CLASS_END_DEBUG

		ACS_LOG_WARNING( e.what() );
		ACS_LOG_WARNING("Exception got trying to compute geographic localization in [" <<setprecision(17) << start << ", " << stop << "] interval: going to clear geo points set so far computed, if any");
		geoPoints.clear();
	} 

	return geoPoints;
}

#ifdef ACS_TEST_GEO_POINT_ITERATOR

void GeoPointIterator::initPropagatorStub(long double start, long double stop, long double delta)
{
}

Metadata::GeoPointInfo GeoPointIterator::getGeoPointInfoStub( long double time )
{
	ACS_COND_THROW( time == 123.0, exIllegalValueException("THIS IS A TEST!") );
	return Metadata::GeoPointInfo();
}

#else

void GeoPointIterator::initPropagator(long double start, long double stop, long double delta)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	double startForInit = start - delta;
	double stopForInit = stop + delta;
	try {
		_propagator->initPropagFromFile(PropagFormat::OP_TIMEMOD_FOS_PREDICTED, startForInit, stopForInit);
	}
	catch(exception& e)
	{
		ostringstream exMsg;
		exMsg << "Unable to initialize OrbitPropagator for interval [" << setprecision(12) << startForInit << "," << stopForInit << "]" ;
		ACS_THROW( exIllegalValueException( e, exMsg.str() ) );
	}

	long double valStart = 0.0;
	long double valStop = 0.0;
	_propagator->getValidationInterval(valStart, valStop);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Initialization Requested[" <<setprecision(17) << startForInit // PRQA S 4400
 			<< "," << stopForInit<< "] -> Validation Interval[" << valStart << "," << valStop << "]") ;
	if( ( valStart > start ) || ( valStop < stop ) ) {
		ostringstream err;
		err << "Initialization interval ["
			<< setprecision(17) << valStart << "," << valStop << "] is lower than requested initialization  interval [" // PRQA S 4400 2
			<< setprecision(12) << start << "," << stop << "] -> ERROR : It is necessary to tune the delta[" 
			<< delta << "]" ;
		ACS_THROW(exIllegalValueException(err.str())); // PRQA S 3081
	}


}

Metadata::GeoPointInfo GeoPointIterator::getGeoPointInfo( StateVector & vec, long double time )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	
	vec.setProcessingTimeValue(time);
	_propagator->propag(vec);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "propag executed with time "  << setprecision(12) << time) ; // PRQA S 4400

	ExtraInfo extraInf;
	_propagator->propagExtraInfo(extraInf);
	double latitude = extraInf.modelIndependent()[PropagFormat::OP_PROPAG_EXTRA_GEOD_LAT];
	double longitude = extraInf.modelIndependent()[PropagFormat::OP_PROPAG_EXTRA_GEOC_LONG];
	if(longitude > 180) {   // PRQA S 4400 2
		longitude -= 360.0;
    }
	
	Metadata::GeoPointInfo point;
	{
		ostringstream os;
		os << latitude;
		point.latitude = os.str();
	}
	
	{
		ostringstream os;
		os << longitude;
		point.longitude = os.str();
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning Geo Point(lat, lon) [" << setprecision(12) << point.latitude << "," << point.longitude << "]") ;
	return point;
}
 #endif



_ACS_END_NAMESPACE
