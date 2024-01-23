// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2014/04/08 16:37:39  marpas
	Library completely rewritten - now perfo are up to 300x faster and accuracy is guaranteed to nanosecs.
	Interface rationalized
	coverage test rewritten
	performance test rewritten
	
	Revision 5.3  2014/02/13 12:13:22  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	
	Revision 5.2  2013/10/17 13:36:48  marpas
	qa warning removed
	
	Revision 5.1  2013/07/23 12:08:58  marpas
	interface rationalization
	performances improved
	fixed timezone bug
	test imaproved
	
	Revision 5.0  2013/07/01 15:28:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.13  2013/04/23 14:21:46  marpas
	getting rid of libFeatures
	
	Revision 2.12  2012/02/08 09:54:57  marpas
	refactoring
	
	Revision 2.11  2012/02/08 09:52:54  marpas
	refactoring
	
	Revision 2.10  2011/09/29 14:57:06  marpas
	getting rid of useless template (causing codecheck crash)
	
	Revision 2.9  2011/04/04 16:32:56  marpas
	mproved efficiency on intersect with no filled time ranges
	
	Revision 2.8  2011/03/31 12:17:20  crivig
	intersects modified to handle ANY
	
	Revision 2.7  2010/05/05 10:20:00  marpas
	DEBUG added
	bug fixed: extraction operator failed with only _start or _stop in the string
	
	Revision 2.6  2009/06/26 10:39:13  marpas
	robustness improved
	
	Revision 2.5  2009/06/23 14:23:24  marpas
	intersects implemented
	
	Revision 2.4  2009/05/22 15:29:14  marpas
	ome op0erational method added (WARNING TimeRange::intersects is not implemented)
	
	Revision 2.3  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.2  2007/06/14 10:20:17  marpas
	anyTimeRange method implemeted for defence backward compatibility
	uses libFeatures
	
	Revision 2.1  2007/06/09 18:22:33  marpas
	wholeDay method added
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.2  2004/09/08 13:55:39  marpas
	removed unnecessary includes
	
	Revision 1.1  2004/08/03 13:48:25  marfav
	Added TimeRange class to libDateTime
	

*/


#include <TimeRange.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TimeRange) 


TimeRange::TimeRange() :
	_start(false), // not filled
	_stop(false)   // not filled
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}



TimeRange::TimeRange(
	const DateTime &start, 
	const DateTime &stop
) :
	_start(start),
	_stop(stop) 
{  
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

TimeRange::TimeRange(const TimeRange &gtr) :
    _start(gtr._start),
    _stop(gtr._stop)

{  
	// empty
}

TimeRange::~TimeRange() throw() {}


TimeRange &TimeRange::operator=(const TimeRange &gtr) 
{
	if (this != &gtr) {
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
		_start = gtr._start ;
		_stop = gtr._stop ;
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	}
	
	return *this ;
}


void TimeRange::start(const DateTime &v) // PRQA S 4121
{ 
	_start = v ;
}

void TimeRange::stop(const DateTime &v)  // PRQA S 4121
{ 
	_stop = v ; 
}


ostream &TimeRange::getSqlString(ostream &os) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	// outputs into a stream the actual time range
	if(_start.isFilled() && _stop.isFilled()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_start & _stop filled both") ;
		// start and stop are both filled
		os << '(' ;
		// writes them in sort order
		if (_start > _stop) {
			_stop.getSqlString(os) ;
			os << ", " ;
			_start.getSqlString(os) ;	
		}
		else {
			_start.getSqlString(os) ;
			os << ", " ;
			_stop.getSqlString(os) ;	
		}
		os << ')' ;
			
	} else if(_start.isFilled()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_start filled") ;
		// here only start is filled, so writes it
		os << '(' ;
		_start.getSqlString(os) ;
		os << ')' ;
	} else if(_stop.isFilled()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_stop filled") ;
		// ... or only stop is filled, writes it
		os << '(' ;
		_stop.getSqlString(os) ;
		os << ')' ;
	}
	else { // either _start and _stop are not filled-> writes ANY
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "none filled") ;
		os << "ANY" ; 
	}
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return os ; // PRQA S 4028
}	



void TimeRange::reset() 
{ 
	_start.reset() ; 
	_stop.reset() ;
}

bool TimeRange::isFilled() const
{ 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_start.isFilled(): " << boolalpha << _start.isFilled()
			   << " _stop.isFilled(): " << boolalpha << _stop.isFilled() ) ;
	return (_start.isFilled() || _stop.isFilled()) ;
}


bool TimeRange::contains(const DateTime &x, bool strict) const // PRQA S 4020
{
	if (!x.isFilled() || !_start.isFilled() || !_stop.isFilled()) { 
		return false ;
    }
	if (_start <= _stop) {
    	return strict ? ((_start < x) && (x < _stop)) : ((_start <= x) && (x <= _stop)) ; // PRQA S 3382, 3385
    }
    else {
    	return strict ? ((_stop < x) && (x < _start)) : ((_stop <= x) && (x <= _start)) ; // PRQA S 3382, 3385
    }
}

bool TimeRange::contains(const DateTime &x, IntervalClosure ic) const // PRQA S 4020
{
	if (!x.isFilled() || !_start.isFilled() || !_stop.isFilled()) {
		return false ;
    }
	
	switch(ic) {
	case OpenInterval:
        {
	        if (_start <= _stop) {
		        return (_start < x) && (x < _stop) ;
            }
            else {
		        return (_stop < x) && (x < _start) ;
            }
        }
	case LeftClosedInterval:
        {
	        if (_start <= _stop) {
		        return _start <= x && x < _stop ;
            }
            else {
		        return _stop <= x && x < _start ;
            }
        }
	case RightClosedIntrval: 
		{
	        if (_start <= _stop) {
                return _start < x && x <= _stop ;
            }
            else {
                return _stop < x && x <= _start ;
            }
        }
	case ClosedInterval:
		{
	        if (_start <= _stop) {
                return _start <= x && x <= _stop ;
            }
            else {
                return _stop <= x && x <= _start ;
            }
        }
	default:
		{
			ACS_THROW(exIllegalValueException("Invalid value for IntervalClosure: " + toString(ic))) ; // PRQA S 3081
		}
	}
}


bool TimeRange::intersects(const TimeRange &tr, IntervalClosure my, IntervalClosure rh) const // PRQA S 4020
{
	if (!tr.isFilled() || !isFilled()) { return true; }

	if (contains(tr.start(), my) || contains(tr.stop(), my)) {
		return true ;
    }
	if (tr.contains(start(), rh) || tr.contains(stop(), rh)) {
		return true ;
    }
	return false ;
}

TimeRange TimeRange::wholeDay() const
{
	// computes an interval starting from 00:00:00 to .. 
	DateTime::UTCD utstart = _start ; // PRQA S 3050 
	utstart.hour = 0;
	utstart.min = 0;
	utstart.sec = 0;
	utstart.nanosec(0) ;
	
	// 23:59:59.999999
	DateTime::UTCD utstop = _stop ;  // PRQA S 3050 
	utstop.hour = 23; // PRQA S 4400 4
	utstop.min = 59;
	utstop.sec = 59;
	utstop.nanosec(999999999) ;
	
    return TimeRange(utstart, utstop) ;
}

TimeRange TimeRange::anyTimeRange() // PRQA S 4020
{
    static TimeRange any ;
    return any ;
} 

const DateTime &TimeRange::start() const {return _start ;} // PRQA S 4120

const DateTime &TimeRange::stop() const {return _stop ;} // PRQA S 4120



istream & operator >> (istream &is, TimeRange& r) // PRQA S 4020
{
    ACS_FUNC_ANNOUNCE_DEBUG(TimeRange,ACS_LOW_VERB) ;

	if (!is.good()) { return is ; } // PRQA S 4028
	
	char ell = 0 ;
	is >> ell ; 
	if (ell == '(') {
		char comma = 0 ;
		acs::DateTime l; 
		is >> l ;
		if (!is.fail()) {
			r.start(l) ;
        }
        acs::DateTime L ;
		is >> comma >> L >> ell ;
		if (!is.fail()) {
			r.stop(L) ;
		}
        ACS_FUNC_WRITE_DEBUG_TH(TimeRange,ACS_LOW_VERB, "r.isFilled(): " << boolalpha << r.isFilled()) ;
		return is ;// PRQA S 4028
	}
	if ((ell == 'A') || (ell == 'a')) { // A of A N Y
		char n = 0 ;
		is.get(n) ;
		if (is.fail()) {
			return is ;// PRQA S 4028
        }
		if (!((n == 'N') || (n=='n'))) { // N of A N Y
			is.putback(n) ;
			is.setstate(ios::failbit) ;
		}
			
		char y = 0 ;
		is.get(y) ;
		if (is.fail()) {
			return is ;// PRQA S 4028
        }
		if (!((y == 'Y') || (y=='y') )) { // Y of A N Y
			is.putback(y) ;
			is.setstate(ios::failbit) ;
		}
		
		r = TimeRange::anyTimeRange() ;
		ACS_FUNC_WRITE_DEBUG_TH(TimeRange,ACS_LOW_VERB, "r.isFilled(): " << boolalpha << r.isFilled()) ;
		return is ;// PRQA S 4028
	}
	ACS_FUNC_WRITE_DEBUG_TH(TimeRange,ACS_LOW_VERB, "WRONG") ;
	
	is.putback(ell) ;
	is.setstate(ios::failbit) ;
	return is ;// PRQA S 4028
}


string TimeRange::toString(TimeRange::IntervalClosure ic)  // PRQA S 4020
{
	switch(ic) {
    case OpenInterval:
        { return "OpenInterval" ; }
	case LeftClosedInterval:
        { return "LeftClosedInterval" ; }
	case RightClosedIntrval:
        { return "RightClosedIntrval" ; }
	case ClosedInterval:
        { return "ClosedInterval" ; }
    default:
        { 
            ostringstream os ;
            os << "<unknown IntervalClosure: " << int(ic) << ">" ; // PRQA S 3081
            return os.str() ; 
        }
	} 
}


exostream & operator << (exostream &os , TimeRange::IntervalClosure ic) // PRQA S 4222
{
    os << TimeRange::toString(ic) ;
    return os ; // PRQA S 4028
}

ostream & operator << (ostream &os , TimeRange::IntervalClosure ic ) 
{
    os << TimeRange::toString(ic) ;
    return os ; // PRQA S 4028
}
_ACS_END_NAMESPACE
