// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
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
    BandShaper() ;
    virtual ~BandShaper() throw () ;
    void registerWorker(const BandPriority &) throw() ;
    void unregisterWorker(const BandPriority &) throw() ;

    typedef unsigned long long Speed_bps ;
    Speed_bps getBandLimit (const BandPriority&) const throw(); // bps;
	void computeUnitPriorityBandLimit() throw();
	void setLinkSpeed(int sp) throw(); // argument must be in Mbps
    void setIsDirect( bool tof ) throw() ;
    bool enabledBandShaping()const throw(){return 0 != _linkSpeed;}
private: // methods
    BandShaper(const BandShaper &) ; // not implemented
    BandShaper& operator=(const BandShaper &) ; // not implemented
	
private: // data
    unsigned long long _linkSpeed;
	std::set<const BandPriority *> _workersRegister;
    Mutex mutex_ ;
	double _unitPriorityBandLimit;
	bool isDirect_;
    
	static const unsigned long long DefaultLinkSpeed ; // 0 Mbps  (no band shaping will be taken in account) // PRQA S 3084, 4400
    ACS_CLASS_DECLARE_DEBUG_LEVEL(BandShaper) ;
} ;

typedef pattern::Singleton<BandShaper > BandShaperSingleton ;


/*
 *
 */
class BandPriority { // PRQA S 2109
public:
    typedef BandShaper::Speed_bps Speed_bps ;

    explicit BandPriority(long prio=0) : priority_(prio) { BandShaperSingleton::instance()->registerWorker(*this) ; } 
    virtual ~BandPriority() throw ()  { BandShaperSingleton::instance()->unregisterWorker(*this) ; } // PRQA S 2131, 4631
    
    bool isEnabled() const throw() { return BandShaperSingleton::instance()->enabledBandShaping(); } // PRQA S 4214
    void setBandPriority(long prio) throw();
    long getBandPriority() const throw() { return priority_ ; }
    Speed_bps getBandLimit () const throw() { return BandShaperSingleton::instance()->getBandLimit(*this) ; } // bps; // PRQA S 4214

private: 
    BandPriority& operator=(const BandPriority &); // not implemented
    BandPriority(const BandPriority &) ; // not implemented
    
private:
    long priority_ ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(BandPriority) ;

};


_ACS_END_NESTED_NAMESPACE

#endif /* _BandPriority_H_ */
