// PRQA S 1050 EOF
/*
	Copyright 013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/04/03 09:02:04  lucio.pulvirenti
	fillAdjacentMatching method implemented because adjacent element of vector with the same value
	of POD.first element are not filled.
	
	Revision 2.2  2013/03/28 15:18:09  lucio.pulvirenti
	Class tested.
	
	Revision 2.1  2013/03/27 17:35:05  lucio.pulvirenti
	First issue.
	
        
*/

#ifndef _OrbitDataMatcher_H_
#define _OrbitDataMatcher_H_


#include <XMLDataConsumer.h>
#include <PredictedOrbitData.h>
#include <exException.h>

#include <cmath>
#include <boost/shared_ptr.hpp>                 // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

class OrbitDataMatcher : public XMLDataConsumer // PRQA S 2109, 2153
{

public:
    typedef boost::shared_ptr<PredictedOrbitData> PODP ;
    typedef std::pair<double, PODP> POD ;
    typedef std::vector<POD > PODs ;

public:
		
	exDECLARE_EXCEPTION(exOrbitDataMatcherException, exException) ;  // PRQA S 2131, 2502

	OrbitDataMatcher(PODs &, long double range) ;
	virtual ~OrbitDataMatcher() throw() ;
	void fillAdjacentMatching() ;
    enum PODField { taiTime, utcTime, ut1Time, xPosition , yPosition, zPosition, xVelocity, yVelocity, zVelocity, absOrbit} ;
protected:
    using XMLDataConsumer::nodeMatch ;
    using XMLDataConsumer::nodeValue ;
    virtual bool nodeMatch(XMLExtractorHandlerBase &, const std::string &parent, const std::string &node, const XERCES_CPP_NAMESPACE::AttributeList &att, const XMLCh* name = 0) ;	
    virtual bool nodeValue(XMLExtractorHandlerBase &, const std::string &parent, const std::string &node, const std::string &value, const XMLCh* name = 0) ;
private:
    OrbitDataMatcher() ;
    OrbitDataMatcher(const OrbitDataMatcher &) ;
    OrbitDataMatcher & operator= (const OrbitDataMatcher &) ;
private:
	PODs &_pods ;
    PODP _actual ;
    bool _extract ; 
    PODField _field ;
    long double range_ ;
    PODs::iterator lastNodeFound_ ;
    PODs::iterator actualFound_ ;
    long double minBound_ ;
    long double maxBound_ ;
		
    ACS_CLASS_DECLARE_DEBUG_LEVEL(OrbitDataMatcher);

} ;


_ACS_END_NAMESPACE

#endif //_OrbitDataMatcher_H_

