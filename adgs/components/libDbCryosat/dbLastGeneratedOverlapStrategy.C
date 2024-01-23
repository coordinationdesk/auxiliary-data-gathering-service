/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:00  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:28  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:49  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/02/24 11:57:02  marpas
	compute overlap algorithm improved:
	1) it now deals with multiple zero-length files
	2) clamping timeline can enlarge it
	
	Revision 1.8  2005/02/23 17:53:47  fracar
	BUG 736 fixed
	
	Revision 1.7  2004/12/30 22:48:00  paoscu
	headers fixed
	
	Revision 1.6  2003/05/20 14:20:24  paoscu
	std namespace.
	
	Revision 1.5  2003/04/17 16:36:34  paoscu
	ostrstream turned into ostringstream.
	
	Revision 1.4  2003/02/27 17:04:08  paoscu
	Uneven case with all input files with the same (zero length) validity interval
	
	Revision 1.3  2003/02/13 16:16:25  paoscu
	Overloaded computeOverlap implemented
	
	Revision 1.2  2003/01/14 14:05:04  paoscu
	Bugs fixed, debug prints added.
	
	Revision 1.1  2003/01/10 20:17:13  paoscu
	dbLastGeneratedOverlapStrategy class added
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#include <dbLastGeneratedOverlapStrategy.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>
#include <sstream>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbLastGeneratedOverlapStrategy);


struct less_datetime {
	bool operator()(const DateTime &, const DateTime &) ;
} ;

bool less_datetime::operator()(const DateTime &a, const DateTime &b)
{
	return a < b ;
}


//
// dbLastGeneratedOverlapStrategy::dbLastGeneratedOverlapStrategy() :
//
dbLastGeneratedOverlapStrategy::dbLastGeneratedOverlapStrategy(const dbGeneralQuery &query) :
	dbOverlapStrategy(query) 
{

}

//
// dbLastGeneratedOverlapStrategy::~dbLastGeneratedOverlapStrategy()
//
dbLastGeneratedOverlapStrategy::~dbLastGeneratedOverlapStrategy() throw() 
{
}

vector< pair <int, dbGeoTimeRange> >& dbLastGeneratedOverlapStrategy::computeOverlap(vector< pair <int, dbGeoTimeRange> >& timeline) 
{
	// check if the query is a suitable one
	const dbInventoryObjectQuery *invQuery = dynamic_cast<const dbInventoryObjectQuery *>(&_query) ;
	if (!invQuery) { // wrong one
		ACS_THROW(WrongQueryTypeException("Query is nor of dbInventoryObjectQuery type neither inherited from it.")) ; // PRQA S 3081
	}
	
	vector< pair <int, dbGeoTimeRange> > result ;
	
	if (invQuery->size() == 0) {
		timeline = result ;
		return timeline ;
	}

	// find all milestones: start and stop of every interval and put them into a temporary vector.
	// Because can potentially exists more than a single file having exactly
	// the same start time or stop, we have to deal with this case too

	vector<DateTime> milestones ;
	
	for (int i=0; i < int(invQuery->size()); i++) {
		const dbInventoryObject *theObj = (*invQuery)[i] ;
		const DateTime &start = theObj->getValidityStart() ;
		const DateTime &stop = theObj->getValidityStop() ;
		
		// Here, if the interval is zero length, we need to insert it completely, 
		// this means that we will have twice the same time value, 
		// to ensure a that zero lenght interval will be in the milestones array
		if (start == stop) { // zero length interval
			if (find(milestones.begin(),milestones.end(),start) == milestones.end()) {
				// just to be clear
				// they are the same, a zero lenght interval is inserted
				milestones.push_back(start) ;
				milestones.push_back(stop) ;
			}
		}
		else {
			if (find(milestones.begin(),milestones.end(),start) == milestones.end()) 
				milestones.push_back(start) ;
			if (find(milestones.begin(),milestones.end(),stop) == milestones.end()) 
				milestones.push_back(stop) ;
		}
	}
			
	// now we have to sort milestones...
	sort(milestones.begin(), milestones.end(), less_datetime()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "milestones.size() ="<< milestones.size()) ;

	
	// between each milestone exists a unique interval that 
	// needs to be assigned to an object in the query, 
	// exactly to the one having newer generation time (last generated)
	// so first of all we fill the result vector assigning 
	// a -1 to each interval.
	// By construction, intervals are sorted in cronological order.
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "milestones") ;
	vector< pair <int, dbGeoTimeRange> > tmp ;
	for (int i=0; i < int(milestones.size())-1; i++) {
		pair <int, dbGeoTimeRange> p ;		
		p.first = -1;
		p.second = dbGeoTimeRange(milestones[i], milestones[i+1]) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, milestones[i].asISO8601(6) << " " << milestones[i+1].asISO8601(6)) ;
		tmp.push_back(p) ;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "tmp.size() ="<< tmp.size()) ;
	// and finally for each interval, we need to assign them to objects. 
	// Note that the object must cover the interval and not just intersect it, 
	// because the presence of zero length intervals
	for (int i=0; i < int(invQuery->size()); i++) {
		const dbInventoryObject *theObj = (*invQuery)[i] ;
		dbGeoTimeRange theObjInt = dbGeoTimeRange(theObj->getValidityStart(), theObj->getValidityStop()) ;
		for (int j=0; j < int(tmp.size()); j++) {
			if (coverInterval(theObjInt, tmp[j].second)) {
				// theObj covers the interval. By construction the coverage is a complete overlap !
				// So we need just to assign the interval if:
				//   1) it is not assigned 
				//   2) it is assigned to an older generated object.
				// Note that in case a zero lenght interval is adjacent to another interval, the object who
				// generated the zero lenght interval will cover the zero lenght interval but not the other one
				// In this case, if the adjacent interval is not zero lenght, the object who covers this one will 
				// by definition, cover the zero lenght too, so the assegnation of both the intervals depends only
				// on object's generation time. 
				// There could be the case that adjacent intervals could be assigned to the same object, the sequence 
				// will be rearranged by next steps
				if (tmp[j].first < 0 || (theObj->getGenerationTime() > (*invQuery)[(tmp[j].first)]->getGenerationTime())) {
					tmp[j].first = i ;
                }
			}
		}
	}
	
	
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		// debug output
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "intervals") ;
		for (size_t i=0; i < tmp.size(); ++i) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "  " << tmp[i].first << "  " <<
			    tmp[i].second.start().asISO8601(6) << ' ' << tmp[i].second.stop().asISO8601(6)) ;
		}
	ACS_CLASS_END_DEBUG
			
	// prepare final result.
	if (tmp.size() == 1) {
		// only one interval, no need to rearrange it
		result = tmp ;
	}
	else {
		// Merging adjacent intervals belonging to the same obj index (-1 included)
		// startSeq will contain the starting of an intervals sequence belonging to the same object
		int startSeq = 0 ;
		
		// loop over every interval but the first one
		for (int i=1; i <= int(tmp.size()); i++) {
			// if we have passed the end of the array or
			// the current interval belongs to a different object that the one who started the sequence ... 
			if (i == int(tmp.size()) || tmp[i].first != tmp[startSeq].first) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "i="<<i) ;
				// ... the whole sequence will generate a single interval with:
				pair <int, dbGeoTimeRange> p ;	
				// A) the object index who owns it ... (the same for the whole sequence)	
				p.first = tmp[startSeq].first;
				// B) the start time of the final interval being the start time of the first interval
				//    and the stop time of the final interval being the stop time of the last interval of the sequnece
				p.second = dbGeoTimeRange(tmp[startSeq].second.start(), tmp[i-1].second.stop()) ;
				result.push_back(p) ;
				// reassign the start sequence index to the first interval who stopped the previous sequence
				startSeq = i ;
			}
		}
	}

	timeline = result ;

	return timeline ;
}

vector< pair <int, dbGeoTimeRange> >& dbLastGeneratedOverlapStrategy::computeOverlap(vector< pair <int, dbGeoTimeRange> >& timeline, const dbGeoTimeRange &clamp) 
{

	// first of all we need to compute the overlapping timeline ...              
	computeOverlap(timeline) ;
	
	// if the clamp interval is not filled .... nothing to do
	if (!clamp.start().isFilled() || !clamp.stop().isFilled()) {
		return timeline ;
    }
		
	// just to avoid doing complicated things with an empty timeline
	if (timeline.empty()) {
		return timeline ;
    }
		
		
	DateTime start(clamp.start()) ;
	DateTime stop(clamp.stop()) ;
		
	// now clamp the timeline with the given clamp range
	// first we need to find the interval containing the start.
	
	// CHECK #1
	// control if the clamp range starts after the end of the timeline or if it ends before its beginning
	if (start > timeline[timeline.size()-1].second.stop() || stop < timeline[0].second.start() ) {
		// clamp interval truncating the timeline
		timeline.clear() ;
		return timeline ;
	}
	
	bool enlarged = false ;
	bool mergeSequenceIfEnlarged = false ;
	if (start > timeline[0].second.start()) {
		// the clamp start is after the beginning of the timeline
		for (size_t i=0; i < timeline.size(); i++) {
			// find the interval containing the clamp start
			if (timeline[i].second.contains(start) && timeline[i].second.stop() != start) {
				// clamp the found interval and erase all the previous intervals.
				// changing the start time of the timeline's interval
				timeline[i].second.start(start) ;
				if (i) // it was not the first ... erase up to the changed one
					timeline.erase(timeline.begin(),timeline.begin()+i) ;
				break ;
			}
		}
	}
	else if (start < timeline[0].second.start()) {
		if (timeline[0].first < 0) // first was not assigned 
			mergeSequenceIfEnlarged = true ; // remember to merge the sequence
			
		// the clamp start was BEFORE the timeline start
		// because the CHECK #1 the clamping stop CANNOT be before the timeline start
		// push a new clamping (enlarging) interval
		timeline.insert(timeline.begin(),1,pair<int, dbGeoTimeRange>(-1,dbGeoTimeRange(start,timeline[0].second.start()))) ; 		
		enlarged = true ;
	}

	// and now the stop
	if (stop < timeline[timeline.size()-1].second.stop()) {
		// the clamping stop would truncate the timeline
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stop: " << stop.asISO8601(6)) ;
		
		for (size_t i=0; i < timeline.size(); i++) {
			// loop over the timeline's intervals to find which one contains the clamping stop
			// the second part of the if should be not necessary because intervals are sorted 
			// cronologically, anyway ... 
			if (timeline[i].second.contains(stop) && timeline[i].second.start() != stop) {
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "[" << i << "] ") ;
				// found, adjust the end of the interval to the clamping stop
				timeline[i].second.stop(stop) ;
				// truncate the timeline if the found one was not the last
				if (i < timeline.size()-1) {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Removing from" << i+1 << " to " << timeline.size()) ;
					timeline.erase(timeline.begin()+i+1, timeline.end()) ;
				}
				break ;
			}
		}
	}
	else if (stop > timeline[timeline.size()-1].second.stop()) {
		if (timeline[timeline.size()-1].first < 0) // last was not assigned 
			mergeSequenceIfEnlarged = true ; // remember to merge the sequence
		// the clamp stop will enlarge the timeline
		// because the CHECK #1 the clamping start CANNOT be past the timeline end
		// simply add a new interval after the timeline end 
		timeline.push_back(pair<int, dbGeoTimeRange>(-1,dbGeoTimeRange(timeline[timeline.size()-1].second.stop(),stop))) ;
		enlarged = true ;
	}
	
	// if the timeline was enlarged, we need to check if the first and last interval of the timeline 
	// have already been assigned, if not, try to assign them to the object query.
	if (enlarged) {
		const dbInventoryObjectQuery *invQuery = dynamic_cast<const dbInventoryObjectQuery *>(&_query) ;
		if (!invQuery) { // wrong one
			ACS_THROW(WrongQueryTypeException("Query is nor of dbInventoryObjectQuery type neither inherited from it.")) ; // PRQA S 3081
		}
		// the above check should not be necessary because has already been done in the 
		// computeOverlap(vector< pair <int, dbGeoTimeRange> >& ) method called at the very beginning of the current one,
		// it is still here just as memento ...
		
		// and finally for each object, we need to assign them to objects. 
		// Note that the object must cover the interval and not just intersect it, because the presence of 
		// zero length intervals
		pair <int, dbGeoTimeRange> & first = timeline[0] ;
		pair <int, dbGeoTimeRange> & last = timeline[timeline.size()-1] ;
		for (int i=0; i < int(invQuery->size()); i++) {
			const dbInventoryObject *theObj = (*invQuery)[i] ;
			dbGeoTimeRange theObjInt = dbGeoTimeRange(theObj->getValidityStart(), theObj->getValidityStop()) ;
			
			if (coverInterval(theObjInt, first.second)) {
				// theObj covers the interval. By construction the coverage is a complete overlap !
				// So we need just to assign the interval if:
				//   1) it is not assigned 
				//   2) it is assigned to an older generated object.
				// Note that in case a zero lenght interval is adjacent to another interval, the object who
				// generated the zero lenght interval will cover the zero lenght interval but not hte other one
				// In this case, if the adjacent interval is not zero lenght, the object who covers this one will 
				// by definition, cover the zero lenght too, so the assegnation of both the intervals depends only
				// on object's generation time. 
				// There could be the case that adjacent intervals could be assigned to the same object, the sequence 
				// will be rearranged by next steps
				if (first.first < 0 || (theObj->getGenerationTime() > (*invQuery)[(first.first)]->getGenerationTime()))
					first.first = i ;
				// remeber to merge the sequence	
				mergeSequenceIfEnlarged = true ;
			}
			if (coverInterval(theObjInt, last.second)) {
				// theObj covers the interval. By construction the coverage is a complete overlap !
				// So we need just to assign the interval if:
				//   1) it is not assigned 
				//   2) it is assigned to an older generated object.
				// Note that in case a zero lenght interval is adjacent to another interval, the object who
				// generated the zero lenght interval will cover the zero lenght interval but not hte other one
				// In this case, if the adjacent interval is not zero lenght, the object who covers this one will 
				// by definition, cover the zero lenght too, so the assegnation of both the intervals depends only
				// on object's generation time. 
				// There could be the case that adjacent intervals could be assigned to the same object, the sequence 
				// will be rearranged by next steps
				if (last.first < 0 || (theObj->getGenerationTime() > (*invQuery)[(last.first)]->getGenerationTime()))
					last.first = i ;
				// remeber to merge the sequence	
				mergeSequenceIfEnlarged = true ;
			}

		} 
		
		
		// Need to merge first two or last two elements
		if (mergeSequenceIfEnlarged) {
			if (timeline.size() > 1) {
				// check the 1st and 2nd element to see if the need merge
				if (timeline[0].first == timeline[1].first) {
					// they DO
					timeline[1].second.start(timeline[0].second.start()) ;
					// erase 1st element
					timeline.erase(timeline.begin()) ;
				}
			}
			if (timeline.size() > 1) {
				// check the last and previous element require merge
				if (timeline[timeline.size()-1].first == timeline[timeline.size()-2].first) {
					// they DO
					timeline[timeline.size()-2].second.stop(timeline[timeline.size()-1].second.stop()) ;
					// erase last element
					timeline.erase(timeline.begin()+timeline.size()-1) ;
				}
			}
		}
	}
	

	return timeline ;
	
}


bool dbLastGeneratedOverlapStrategy::intersectInterval(const dbGeoTimeRange &a, const dbGeoTimeRange &b)
{
	const DateTime &astart = a.start() ;
	const DateTime &astop = a.stop() ;
	const DateTime &bstart = b.start() ;
	const DateTime &bstop = b.stop() ;
	
	return (astart <= bstart &&  bstart < astop) || (bstart <= astart && astart < bstop) ;
}	



bool dbLastGeneratedOverlapStrategy::coverInterval(const dbGeoTimeRange &cover, const dbGeoTimeRange &tbcovered )
{
	return (cover.start() <= tbcovered.start() &&  cover.stop() >= tbcovered.stop()) ;
}	
_ACS_END_NAMESPACE
