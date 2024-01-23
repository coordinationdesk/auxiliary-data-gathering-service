/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/04/08 16:37:39  marpas
	Library completely rewritten - now perfo are up to 300x faster and accuracy is guaranteed to nanosecs.
	Interface rationalized
	coverage test rewritten
	performance test rewritten
	
	Revision 5.0  2013/07/01 15:28:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.12  2012/12/02 20:04:10  marpas
	qa rules
	
	Revision 2.11  2012/02/08 09:52:54  marpas
	refactoring
	
	Revision 2.10  2011/10/04 11:55:31  marpas
	getting rid of useless template for codecheck
	
	Revision 2.9  2011/04/04 16:32:56  marpas
	mproved efficiency on intersect with no filled time ranges
	
	Revision 2.8  2010/05/05 10:20:00  marpas
	DEBUG added
	bug fixed: extraction operator failed with only _start or _stop in the string
	
	Revision 2.7  2009/05/22 15:29:14  marpas
	ome op0erational method added (WARNING TimeRange::intersects is not implemented)
	
	Revision 2.6  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.5  2007/06/14 10:22:24  marpas
	compilation error fixed
	
	Revision 2.4  2007/06/14 10:21:37  marpas
	anyTimeRange used
	
	Revision 2.3  2007/06/14 10:20:17  marpas
	anyTimeRange method implemeted for defence backward compatibility
	uses libFeatures
	
	Revision 2.2  2007/06/09 18:22:33  marpas
	wholeDay method added
	
	Revision 2.1  2006/03/24 08:50:44  marpas
	GCC 3.4.5 complained with inline definition
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/10/22 17:02:12  marpas
	streams extractors and tests
	
	Revision 1.2  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.1  2004/08/03 13:48:25  marfav
	Added TimeRange class to libDateTime
	

*/


#ifndef _TimeRange_H_
#define _TimeRange_H_

#include <acs_c++config.hpp>
#include <DateTime.h>
#include <exException.h>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)


//          This class is a representation of libDbGeoTimeRange used to avoid
//          informix dependance when using time range only

class TimeRange { // PRQA S 2109
    friend std::istream &operator >> (std::istream &is, TimeRange& r) ; // PRQA S 2107
public:

	TimeRange() ;
	TimeRange(const DateTime &, const DateTime &) ;
	virtual ~TimeRange() = default ;

	/** 
	 * Checks if the given datetime is inside the range.
	 * if strict is true, range borders are not included
	 */
	bool contains(const DateTime &, bool strict = false) const ;
	
	enum IntervalClosure { OpenInterval, LeftClosedInterval, RightClosedIntrval, ClosedInterval } ; // NOSONAR
	static std::string toString(IntervalClosure) ; // PRQA S 2502
    
	bool contains(const DateTime &, IntervalClosure) const ;
	bool intersects(const TimeRange &, IntervalClosure, IntervalClosure) const ;
	
	const DateTime &start() const ;
	void start(const DateTime &) ;
	
	const DateTime &stop() const ;
	void stop(const DateTime &) ;

	TimeRange wholeDay() const ;

	std::ostream &getSqlString(std::ostream &) const ;
	bool isFilled() const ; 
	void reset() ;
    TimeRange & invert() { DateTime t = _start ; _start = _stop; _stop = t ; return *this ; }
	static TimeRange anyTimeRange() ; 

private:
    // PRQA S 2101 5    
	DateTime _start ;
	DateTime _stop ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(TimeRange) ;
} ;


std::istream & operator >> (std::istream &is, TimeRange& r) ;

std::ostream & operator<< ( std::ostream &, TimeRange::IntervalClosure) ;
exostream & operator << (exostream &, TimeRange::IntervalClosure) ; // PRQA S 2072

inline bool operator==(const TimeRange& a, const TimeRange &b) // PRQA S 2134
{
    return (a.start() == b.start()) && (a.stop() == b.stop()) ;
}
inline bool operator!=(const TimeRange& a, const TimeRange &b) { return ! (a == b) ; } // PRQA S 2134

_ACS_END_NAMESPACE




#endif

