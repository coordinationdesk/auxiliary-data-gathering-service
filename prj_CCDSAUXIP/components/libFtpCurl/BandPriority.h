/*
 *
 *  Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 1.10  2014/09/10 16:04:54  francesco.avanzi
 *  BandShaper::enabledBandShaping() method added
 *  DefaultLinkSpeed is now initialized in .C file
 *  BandPriority::isEnabled() method added
 *
 *  Revision 1.9  2014/03/13 13:03:10  francesco.avanzi
 *  getIsDirect method removed
 *
 *  Revision 1.8  2014/03/13 11:08:13  francesco.avanzi
 *  getIsDirect() method added
 *
 *  Revision 1.7  2014/03/12 11:10:53  francesco.avanzi
 *  minor issue taken in account
 *
 *  Revision 1.6  2014/03/11 18:44:23  francesco.avanzi
 *  solved issue involving inverse priority logic
 *
 *  Revision 1.5  2014/03/07 11:24:14  francesco.avanzi
 *  Priority inverse logic implemented
 *
 *  Revision 1.4  2014/02/20 14:28:01  lucio.pulvirenti
 *  typedef for bps introduced as unsigned long long int. QA analysisi fixed. BandShaper::getBandLimit now const.
 *
 *  Revision 1.3  2014/02/17 16:56:47  lucio.pulvirenti
 *  Implementation completed. Unit test performed.
 *
 *  Revision 1.2  2014/02/14 17:36:57  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 1.1  2014/02/14 14:03:39  marpas
 *  attempt to refactor band shaping mechanism
 *
 *
 *
 */

#ifndef _BandPriority_H_
#define _BandPriority_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <set>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)


class BandPriority ;

class BandShaper { // PRQA S 2109
public:
    BandShaper() = default ;

    BandShaper(const BandShaper &) = delete ;
    BandShaper& operator=(const BandShaper &) = delete ;

    virtual ~BandShaper() = default ;
    
    void registerWorker(const BandPriority &) noexcept ;
    void unregisterWorker(const BandPriority &) noexcept ;

    using Speed_bps = unsigned long long ;
    
    Speed_bps getBandLimit (const BandPriority&) const noexcept; // bps;
	void computeUnitPriorityBandLimit() noexcept;
	void setLinkSpeed(int sp) noexcept; // argument must be in Mbps
    void setIsDirect( bool tof ) noexcept ;
    bool enabledBandShaping()const noexcept{return 0 != _linkSpeed;}
	
private: 
    /* Default initialization for LinkSpeed: linkSpeed = 0 means unlimited   */
	static const unsigned long long DefaultLinkSpeed = 0 ; // 0 Mbps  (no band shaping will be taken in account) // PRQA S 3084, 4400

    unsigned long long _linkSpeed = DefaultLinkSpeed ;
	std::set<const BandPriority *> _workersRegister = {} ;
    Mutex mutex_ = {} ;
	double _unitPriorityBandLimit = 0 ;
	bool isDirect_ = true ;
    
    ACS_CLASS_DECLARE_DEBUG_LEVEL(BandShaper) ;
} ;

using BandShaperSingleton = pattern::Singleton<BandShaper > ;


/*
 *
 */
class BandPriority { // PRQA S 2109
public:
    using Speed_bps =  BandShaper::Speed_bps  ;

    explicit BandPriority(long prio=0) : priority_(prio) { BandShaperSingleton::instance()->registerWorker(*this) ; } 

    BandPriority& operator=(const BandPriority &) = delete ;
    BandPriority(const BandPriority &) = delete ;

    virtual ~BandPriority() noexcept  { BandShaperSingleton::instance()->unregisterWorker(*this) ; } // PRQA S 2131, 4631
    
    bool isEnabled() const noexcept { return BandShaperSingleton::instance()->enabledBandShaping(); } // PRQA S 4214
    void setBandPriority(long prio) noexcept;
    long getBandPriority() const noexcept { return priority_ ; }
    Speed_bps getBandLimit () const noexcept { return BandShaperSingleton::instance()->getBandLimit(*this) ; } // bps; // PRQA S 4214


    
private:
    long priority_ ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(BandPriority) ;

};


_ACS_END_NESTED_NAMESPACE

#endif /* _BandPriority_H_ */
