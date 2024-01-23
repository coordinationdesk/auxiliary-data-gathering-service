// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2018, Advanced Computer Systems , Inc.
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
 *  Revision 1.15  2014/09/10 16:03:02  francesco.avanzi
 *  DefaultLinkSpeed is now initialized in the .C file
 *
 *  Revision 1.14  2014/06/04 17:29:20  marfav
 *  QA warning removed
 *  HTTP sessions managed without SSL layer
 *  CURL locking callback functions strategy revised
 *
 *  Revision 1.13  2014/03/13 13:03:10  francesco.avanzi
 *  getIsDirect method removed
 *
 *  Revision 1.12  2014/03/13 11:08:13  francesco.avanzi
 *  getIsDirect() method added
 *
 *  Revision 1.11  2014/03/12 12:20:35  marpas
 *  rationalization and qa warnings removed
 *
 *  Revision 1.10  2014/03/12 11:10:53  francesco.avanzi
 *  minor issue taken in account
 *
 *  Revision 1.9  2014/03/12 08:44:59  francesco.avanzi
 *  float now in line with double
 *
 *  Revision 1.8  2014/03/11 18:44:23  francesco.avanzi
 *  solved issue involving inverse priority logic
 *
 *  Revision 1.7  2014/03/07 11:24:14  francesco.avanzi
 *  Priority inverse logic implemented
 *
 *  Revision 1.6  2014/02/21 16:34:57  lucio.pulvirenti
 *  Debug improved.
 *
 *  Revision 1.5  2014/02/20 15:10:53  lucio.pulvirenti
 *  Useless include removed.
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

#include <BandPriority.h>

#include <Filterables.h>
#include <algorithm>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(BandShaper);
/*Default initialization for LinkSpeed: linkSpeed = 0 means unlimited   */
const unsigned long long BandShaper::DefaultLinkSpeed = 0;

BandShaper::BandShaper():
	_linkSpeed(DefaultLinkSpeed),
	_workersRegister(),
    mutex_(),
	_unitPriorityBandLimit(),
    isDirect_(true)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	
}
    
BandShaper::~BandShaper() throw () { }

void BandShaper::setLinkSpeed(int sp) throw() 
{ 
	_linkSpeed = sp * 1000000UL; // bps // PRQA S 3000, 3084, 4400
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Link speed set to " << _linkSpeed << " bps");
}
void BandShaper::setIsDirect( bool tof ) throw() 
{
    isDirect_ = tof;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "isDirect_ set to " << boolalpha << isDirect_ );
}

void BandShaper::registerWorker(const BandPriority & bp) throw() 
{
    Lock l_(mutex_) ; // acquire lock
	_workersRegister.insert(&bp);
	 ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Elements " << hex << &bp << dec << ", worker register size: " << _workersRegister.size());
	 computeUnitPriorityBandLimit();
	
}

void BandShaper::unregisterWorker(const BandPriority & bp) throw() 
{
    Lock l_(mutex_) ; // acquire lock
	_workersRegister.erase(&bp);
	 ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Elements " << hex << &bp << dec << ", worker register size: " << _workersRegister.size());
	 computeUnitPriorityBandLimit();
}

BandShaper::Speed_bps BandShaper::getBandLimit (const BandPriority & bp) const throw() // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);


    if ( bp.getBandPriority() > 0 ) {
        double normalizedBP = isDirect_  ? bp.getBandPriority() : ( 1.0 / bp.getBandPriority() ) ; // PRQA S 3011, 3384
        return normalizedBP *  _unitPriorityBandLimit ; // PRQA S 3011
    
    }
    return _linkSpeed;
}

namespace {
    class PrioAccum // PRQA S 2109
    {
    public:
        PrioAccum (double &accum, bool directPrio) :
            accum_(accum), // PRQA S 2528
            directPrio_(directPrio)
        {}
        
        PrioAccum (const PrioAccum &r) : accum_(r.accum_), directPrio_(r.directPrio_) {} // PRQA S 2528
        ~PrioAccum() {}

        void operator() (const BandPriority *bp)  
        {
            long prio = bp->getBandPriority() ;
            if(directPrio_) {
    		    accum_ += prio ; // PRQA S 3011
	        }
            else {
                if(0 != prio) // only if not 0 - avoid 0-divide
                {
                    accum_ += 1.0 / prio ; // PRQA S 3011
                }
            }
        }
    private:  
        PrioAccum() ; // not implemented 
        PrioAccum & operator= (const PrioAccum &) ; // not implemented
    private:
        double &accum_; // NB: this MUST be a reference where to accumulate sums
        bool directPrio_;
    };
}

void BandShaper::computeUnitPriorityBandLimit() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l_(mutex_) ; // acquire lock
    double prioritySum = 0;
    PrioAccum accu(prioritySum, isDirect_ ) ;
    for_each(_workersRegister.begin(), _workersRegister.end(), accu)  ;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sum of priority over " << _workersRegister.size() << " elements: " << prioritySum );
    
    // the following if seems incorrect because compare for equality
	if( prioritySum > 0.0) {
		_unitPriorityBandLimit = _linkSpeed / prioritySum ; // PRQA S 3011, 3084
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_unitPriorityBandLimit: [" << _unitPriorityBandLimit << "] prioritySum: [" << prioritySum << "] _linkSpeed: " << _linkSpeed);
    }
	else {
		_unitPriorityBandLimit = 0;
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "NOT THINKING ABOUT PRIORITY");
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Band limit unitary fraction: " << _unitPriorityBandLimit << " bps");


}

ACS_CLASS_DEFINE_DEBUG_LEVEL(BandPriority);


void BandPriority::setBandPriority(long prio) throw()
{
	priority_ = prio;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Priority set to: " << priority_ << " for element " << hex << this);
	BandShaperSingleton::instance()->computeUnitPriorityBandLimit() ;
}


_ACS_END_NESTED_NAMESPACE
