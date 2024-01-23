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

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 5.0.2.1  2016/10/26 12:06:26  lucio.pulvirenti
	OPMAN-217: iterate method rewritten since uncorrect. initPropagator and getGeoPointInfo private methods added. ACS_TEST_GEO_POINT_ITERATOR compilation flag
	defined to test iterate method w.r.t of time computing (neither propagation nor geo points computed).
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/02/21 19:25:16  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.4  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.3  2008/08/27 16:50:02  giucas
	Mission parameter added to default 1
	
	Revision 2.2  2006/07/05 08:46:10  giucas
	Old algo reintroduced
	
	Revision 2.1  2006/06/02 13:10:36  giucas
	Introduced automatic delta tuning to cover error cases arised during GOCE V2 DR (the delta was too large)
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.1  2004/11/04 15:37:34  giucas
	Imported source
	

*/

#ifndef _GeoPointIterator_H_
#define _GeoPointIterator_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <Metadata.h>


_ACS_BEGIN_NAMESPACE(acs)

class OrbitPropagator;
class StateVector;

/*
 * Calculate a list of GeoPoints given the Satellite Name, an initialization interval, a propagation step
 * and an initialization margin
 */
class GeoPointIterator
{
public:

	/**
	* \brief Class constructor
	* \param Satellite Name
	* \param Satellite Mission default is 1
	*/
	explicit GeoPointIterator(const std::string&, int mission=1);

	/**
	* \brief  Class destructor
	*/ 
	~GeoPointIterator() throw() ;


	/**
	* \brief Propagate from input start time to stop time at a given step (in seconds) and return the
	*		 list of GeoPoints
	* \param The input start time expressed in days
	* \param The input stop time expressed in days
	* \param The propagation step expressed in seconds
	* \param The initialization margin expressed in days
	*/	
	std::vector<Metadata::GeoPointInfo> iterate(long double,long double,long double,long double);


private:

	GeoPointIterator();
	GeoPointIterator(const GeoPointIterator&);
	GeoPointIterator& operator=(const GeoPointIterator&);

#ifdef ACS_TEST_GEO_POINT_ITERATOR

	void initPropagatorStub(long double start, long double stop, long double delta);
	Metadata::GeoPointInfo getGeoPointInfoStub( long double time );

#else
	OrbitPropagator* 	_propagator;

	void initPropagator(long double start, long double stop, long double delta);
	
	Metadata::GeoPointInfo getGeoPointInfo( StateVector &, long double time );


#endif

	ACS_CLASS_DECLARE_DEBUG_LEVEL(GeoPointIterator) ;
};


_ACS_END_NAMESPACE

#endif //_GeoPointIterator_H_
