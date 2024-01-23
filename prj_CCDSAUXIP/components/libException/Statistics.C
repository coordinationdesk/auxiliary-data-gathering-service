/*
	Copyright 2009-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2018/10/01 14:36:46  marpas
	very long keys in statistics are fitted into the devoted space using first part, three dots and last part.
	For example if "1234567890" is too long it could be shrinked into "12...980"
	
	Revision 5.7  2014/03/21 13:57:17  marpas
	qa warnings
	
	Revision 5.6  2014/02/25 15:53:15  marpas
	Statistics appeareance fixed
	
	Revision 5.5  2013/12/18 13:39:37  marpas
	Statistics output aesthetic make-up
	
	Revision 5.4  2013/10/03 18:03:29  marpas
	Statiticssingleton is not released at exit
	
	Revision 5.3  2013/09/26 18:17:44  marpas
	housekeeping, types renaming and cleanup
	
	Revision 5.2  2013/06/18 12:25:19  marpas
	some include files are now deprecated
	
	Revision 5.1  2013/06/05 15:56:21  marpas
	new macros for real time debugging adopted
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.16  2013/05/14 17:57:20  marpas
	Singleton's release at exit for Statistics
	
	Revision 4.15  2013/04/09 07:46:43  marpas
	statistics are now in math namespace
	
	Revision 4.14  2013/04/04 18:34:37  marpas
	Statistics interface improved (reset, zero, zeroTree, resetTree and clearTree)
	
	Revision 4.13  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.12  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.11  2012/06/22 13:41:46  marpas
	pr qa instrumentation
	
	Revision 4.10  2012/06/14 17:16:40  marpas
	qacpp instrumentation
	
	Revision 4.9  2012/06/12 17:37:00  marpas
	qacpp instrumentation
	
	Revision 4.8  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.7  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.6  2012/06/05 17:07:00  marpas
	resolving some quality issues and documentation
	
	Revision 4.5  2012/03/19 15:49:24  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.4  2012/03/05 17:16:18  marpas
	empty stat tree are not longer printed
	
	Revision 4.3  2012/02/28 11:35:22  marpas
	empty stats are no longer notified
	
	Revision 4.2  2011/10/20 17:55:04  marpas
	improved (1.5 sigma)
	
	Revision 4.1  2011/10/18 17:23:06  marpas
	some display improvements
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.3  2011/04/29 13:09:55  marpas
	Statistics work in progress
	
	Revision 3.2  2011/04/22 14:53:06  marpas
	readability improved
	
	Revision 3.1  2011/03/10 18:57:40  marpas
	still in progress
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	

*/

#include <Statistics.h>
#include <exStream.h>
#include <Filterables.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, math)

namespace {
	// Unary predicate for blank characters
	bool isBlankChar(char c) { return ((c == ' ') || (c == '\t')); }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(StatNode) 



StatNode::StatNode() : 
	_key("<stat>")
{
	_labels.push_back(_key);
	_nodePath = "/" + _key;
}




StatNode::StatNode(const StatNode &rhs) :
	_value(rhs._value ? rhs._value->clone() : nullptr), // PRQA S 3380
	_subtree(rhs._subtree ),
	_key(rhs._key),
	_labels(rhs._labels),
	_nodePath(rhs._nodePath)
{
}


StatNode &StatNode::operator=(const StatNode &rhs) // PRQA S 4250 # no assignement of _mutex
{
	if (this != &rhs) { // avoid autoassignation
		Lock r(rhs) ; // lock both the objects // PRQA S 3050 2
		Lock const l(*this) ;
        // no copy of the mutex
		// deep copy 
        
        _value.reset(rhs._value ? rhs._value->clone() : nullptr) ;
        
		_subtree = rhs._subtree ;
		if (! rhs._key.empty()) { // keep old one if rhs empty
			_key = rhs._key ;
		}

		_labels = rhs._labels;
		_nodePath = rhs._nodePath;
	}

	return *this ;
}




StatNode &StatNode::operator[](const std::string &k) // PRQA S 4020
{
	Lock const l(*this) ; // lock // PRQA S 3050
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "k: " << k) ;
	// find the key
	auto f = _subtree.find(k) ;
	if (f == _subtree.end()) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "creating node " << k << " under: " << _key) ;
		
		StatNode &r = _subtree[k] ;
		Lock const lr(r) ; // lock it // PRQA S 3050

		r._key = k ;
		
		// Replace blank characters in the key with underscores
		std::string keyclone = r._key;
		std::replace_if(keyclone.begin(), keyclone.end(), isBlankChar, '_');
		
		// the new node labels consist of the parent labels plus its own key
		r._labels = _labels;
		r._labels.push_back(keyclone);
		
		// Compute node path by concat labels
		r._nodePath = "/";
		size_t lbsize = r._labels.size();
		for (size_t idx = 0; idx < lbsize; ++idx) {
			r._nodePath += r._labels[idx];
			if (idx < (lbsize - 1)) { // PRQA S 3084
				r._nodePath += "/";
			} 
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "defined node path \"" << r._nodePath << "\" for node key " << keyclone);
		
		return r ; // return it 
	}
	
	return f->second ;
	
}

StatNode &StatNode::reset()
{
    if (_value) {
	    Lock const ll(*this) ;
	    _value.reset() ;
    }
    
	return *this ;
}

StatNode &StatNode::zero()
{
    if (_value) {
	    Lock const ll(*this) ;  // PRQA S 3050
		_value->zero() ;
    }
	return *this ;
}

const std::string &StatNode::getNodePath() const
{
	Lock const ll(*this) ;  // PRQA S 3050
	return _nodePath ;
}

void StatNode::getLabels(std::vector<std::string>& lbl) const  {
	lbl.clear();
	
	Lock const l(*this) ;// PRQA S 3050
	lbl = _labels;
}

void StatNode::zeroTree() 
{
	Lock const l(*this) ;// PRQA S 3050
    zero() ;
    for_each(_subtree.begin(), _subtree.end(), [](auto n) { n.second.zeroTree(); }) ; 
}

void StatNode::resetTree() 
{
	Lock const l(*this) ;// PRQA S 3050
    reset() ;
    for_each(_subtree.begin(), _subtree.end(), [](auto n) { n.second.resetTree(); }) ; 
}

namespace {
    std::string get_key_desc(const std::string &k, int len) // PRQA S 4020
    {
        int l = int(k.length()) ;  // NOSONAR
        if (l > (len-1)) { // key is too long for the space in the output
            int left = (len-3)/2 ;
            // try to get beginning and ending of the key
            return k.substr(0, left) + "..." + k.substr( l - (len - left -3)); // PRQA S 3000, 4400
        }
        // just use the whole key
        return k + ' ';
    }
}

void StatNode::writeTo(std::ostream &s, int level) const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key: " << _key << "["<<level<<"]" << ( _value ? "" : " NO VALUE")) ; // PRQA S 3380
	Lock const l(*this) ; // PRQA S 3050
	s << std::setfill(' ') << std::setw(level*AccuBase::__indent) << "" ;
	// PRQA S 3000,3051,3081,3380 L1
    char fillchar = _value ? '.' : '+' ; // PRQA S 3384
    int fill_len = AccuBase::__titlew - level*AccuBase::__indent ;
    s << std::left << std::setw(fill_len) << std::setfill(fillchar) 
	  << get_key_desc(_key,fill_len) << " " ; 
    // PRQA L:L1
	if (_value) { _value->writeTo(s) ; } // write the value

    s << "\n" ;
    
    for_each(_subtree.begin(), _subtree.end(), 
        [level, &s](auto const i){ i.second.writeTo(s,level+1) ; }) ;
}

unsigned long long StatNode::sum_n() const 
{
	Lock const l(*this) ; // PRQA S 3050
    unsigned long long sumN = 0 ;
    sumN += _value ? _value->n() : 0 ; // PRQA S 3000, 3084,3051,3380
    for_each(_subtree.begin(), _subtree.end(), [&sumN](auto const i){ sumN += i.second.sum_n() ; } ) ;
    
    return sumN ;    
}



// standard inserter implementation, returns the left hand argument in order to 
// chain multiple inserter from left to right.
std::ostream &operator<<(std::ostream &os, const StatNode &a) 
{
	a.writeTo(os,0) ;
	return os ; // PRQA S 4028
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(Statistics) 
			
Statistics::Statistics() : 
    pattern::Singleton<Statistics>()
{}


bool Statistics::enableStatistics(bool v)
{
	bool previous = statsAreEnabled_ ;
	statsAreEnabled_ = v ;
	return previous ;
}

StatNode &Statistics::operator()() { return root_ ; } // PRQA S 4120,4024

void Statistics::getStatistics(std::string &stat) // PRQA S 4020 
{	
	if ((Statistics::instance()->statsAreEnabled_ == true) || (ACS_CLASS_GET_DEBUG() > 0)) {
        if ((*instance())().sum_n() == 0 ) { stat = ""; return ; }
		// prepare the out string
		std::ostringstream os ;
		os << Accumulator<double>::title() << '\n' ; // any accumulator will fit
		os << (*instance())() ;
		os << Accumulator<double>::title(true) << '\n' ; // any accumulator will fit

		stat = os.str() ;
	ACS_CLASS_END_DEBUG
}


_ACS_END_NESTED_NAMESPACE

