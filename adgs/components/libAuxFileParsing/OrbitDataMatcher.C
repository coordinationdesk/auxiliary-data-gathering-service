// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
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

#include <OrbitDataMatcher.h>

#include <DateTime.h>
#include <Filterables.h>

#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrbitDataMatcher)

_ACS_BEGIN_NAMESPACE(auxalgo)
struct match{ // PRQA S 2171, 2173, 2175
    match(long double v, long double range) : v_(v), range_(range) {}

    bool operator()(const std::pair<double, OrbitDataMatcher::PODP> &el) { return fabsl(el.first-v_) <= range_ ; } // PRQA S 4211
private:
    match();

    long double v_ ; // PRQA S 2100 2
    long double range_ ;
} ;

struct PODsMatch{ // PRQA S 2171, 2173, 2175
    explicit PODsMatch(long double range) : range_(range) {}

    bool operator()(const std::pair<double, OrbitDataMatcher::PODP> &el1, 
                   const std::pair<double, OrbitDataMatcher::PODP> &el2 ) const { return fabs(el1.first-el2.first) <= range_ ; } // PRQA S 4211
private:
    PODsMatch();

    long double range_ ;
} ;



bool compare(const std::pair<double, OrbitDataMatcher::PODP> & i, const std::pair<double, OrbitDataMatcher::PODP> &j) { return (i.first<j.first);}

OrbitDataMatcher::PODs & sort_pods(OrbitDataMatcher::PODs &p) {
    ACS_COND_THROW(p.empty(), exIllegalValueException("empty vector")) ; // PRQA S 3081
    std::sort(p.begin(), p.end(), auxalgo::compare) ;
    return p ; // PRQA S 4028
}

_ACS_END_NAMESPACE



OrbitDataMatcher::OrbitDataMatcher(PODs & pods, long double range) :
    XMLDataConsumer(),
	_pods(auxalgo::sort_pods(pods)),
    _actual(),
    _extract(false),
    _field(taiTime),
    range_(range),
    lastNodeFound_(_pods.begin()),
    actualFound_(_pods.end()),
    minBound_(_pods[0].first - range_),
    maxBound_(_pods[_pods.size()-1].first + range_) // PRQA S 3084
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

OrbitDataMatcher::~OrbitDataMatcher() throw() 
{
}

bool OrbitDataMatcher::nodeMatch(XMLExtractorHandlerBase &xh, const string &parent, const string &node, const XERCES_CPP_NAMESPACE::AttributeList &, const XMLCh* name) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"method called with node \"" <<  node << "\"") ;
  
	 _extract = false ;
    if (node == "TAI") {
        _extract = true ;
        _field = taiTime ;
    }
    else if (node == "UTC") {
        _extract = true ;
        _field = utcTime ;
    }
    else if (node == "UT1") {
        _extract = true ;
        _field = ut1Time ;
    }
    else if (node == "X") {
        _extract = true ;
        _field = xPosition ;
    }
    else if (node == "Y") {
        _extract = true ;
        _field = yPosition ;
    }
    else if (node == "Z") {
        _extract = true ;
        _field = zPosition ;
    }
	else if( node.substr(0, 1) == "V" ) {
    	if(node == "VX") {
        	_extract = true ;
        	_field = xVelocity ;
    	}
		else if(node == "VY") {
        	_extract = true ;
        	_field = yVelocity ;
    	}
		else if(node == "VZ") {
        	_extract = true ;
        	_field = zVelocity ;
    	}
        else{}
	}
    else if (node == "Absolute_Orbit") {
        _extract = true ;
        _field = absOrbit ;
    }
    else if (node == "OSV") {
        _actual = PODP(new PredictedOrbitData) ; // PRQA S 3081
    }
    else {}

	return true; 
}
		
bool OrbitDataMatcher::nodeValue(XMLExtractorHandlerBase &xh, const string &parent, const string &node, const string &value, const XMLCh* name)   // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"method called with node \"" <<  node << "\"") ;

    if (node == "OSV") {
    
        if (actualFound_ != _pods.end()) {
            actualFound_->second = _actual ;
        }
        return true ;
    }
    
    if (_extract) {
        switch(_field) {
        case taiTime: 
            {
                DateTime d(value) ;
                _actual->taiTime = DateTime::MJD2000(d).jd() ; // PRQA S 3050, 3081
				break;
            }
        case utcTime: 
            {
                DateTime d(value) ;
                _actual->utcTime = DateTime::MJD2000(d).jd() ;// PRQA S 3050, 3081

                actualFound_ = _pods.end() ;
                
                if (_actual->utcTime < minBound_) {
                    return true ;
                }
                
                if (_actual->utcTime > maxBound_) {
                    return true ;
                } 
               
                PODs::iterator f = find_if( lastNodeFound_, _pods.end(), auxalgo::match(_actual->utcTime,range_)) ;
                if (f != _pods.end()) {
                    actualFound_ = lastNodeFound_ = f ;
                }
				break;
            }
        default:
            {
                if (actualFound_ == _pods.end()) { return true ; }
                break ;
            }
        }

        switch(_field) {
        case ut1Time:
            {
                DateTime d(value) ;
                _actual->ut1Time = DateTime::MJD2000(d).jd() ;// PRQA S 3050, 3081
                break ;
            }
        case xPosition:
            {
                _actual->xPosition = strtod(value.c_str(), 0);
                break ;
            }
        case yPosition:
            {
                _actual->yPosition = strtod(value.c_str(), 0);
                break ;
            }
        case zPosition:
            {
                _actual->zPosition = strtod(value.c_str(), 0);
                break ;
            }
        case xVelocity:
            {
                _actual->xVelocity = strtod(value.c_str(), 0);
                break ;
            }
        case yVelocity:
            {
                _actual->yVelocity = strtod(value.c_str(), 0);
                break ;
            }
        case zVelocity:
            {
                _actual->zVelocity = strtod(value.c_str(), 0);
                break ;
            }
        case absOrbit:
            {
                _actual->absOrbit = strtol(value.c_str(), 0, 10); // PRQA S 4400
                break ;
            }
		 default: {}
        }
    }
	return true ;
}


void OrbitDataMatcher::fillAdjacentMatching() 
{
    for (OrbitDataMatcher::PODs::iterator i=_pods.begin(); i!= _pods.end();) { // PRQA S 4238
        i = adjacent_find(i, _pods.end(), auxalgo::PODsMatch(range_)); // PRQA S 3081
        if ((i !=  _pods.end()) && (i->second.get() != 0) ) {
            OrbitDataMatcher::PODs::iterator p = i ;
            (++i)->second = p->second ;
        }
    }
}


_ACS_END_NAMESPACE

