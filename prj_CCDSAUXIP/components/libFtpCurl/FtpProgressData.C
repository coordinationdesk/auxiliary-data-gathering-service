// PRQA S 1050 EOF
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
 *  Revision 1.11  2014/10/30 10:32:32  marpas
 *  qa warnings removed
 *
 *  Revision 1.10  2014/09/10 16:06:31  francesco.avanzi
 *  now also checking if Band Shaping is enabled
 *
 *  Revision 1.9  2014/06/20 14:52:34  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 1.8  2014/02/26 15:33:37  francesco.avanzi
 *  comment was added to extricate a few tricky passages
 *
 *  Revision 1.7  2014/02/26 12:00:45  francesco.avanzi
 *  del_ evaluation fixed
 *
 *  Revision 1.6  2014/02/26 09:08:00  francesco.avanzi
 *  work in progress
 *
 *  Revision 1.5  2014/02/25 09:35:50  francesco.avanzi
 *  qac++analisys run
 *
 *  Revision 1.4  2014/02/24 20:49:07  marpas
 *  perfo and progress data rationalization
 *
 *  Revision 1.3  2014/02/21 16:47:54  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 1.2  2014/02/20 14:14:04  francesco.avanzi
 *  implementation is over, Test is executed
 *
 *  Revision 1.1  2014/02/14 14:03:39  marpas
 *  attempt to refactor band shaping mechanism
 *
 *
 *
 */

#include <FtpProgressData.h>
#include <BandPriority.h>
#include <Timer.h>
#include <Filterables.h>
#include <algorithm>
#include <iterator>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(FtpProgressData);

FtpProgressData::FtpProgressData(StopController const& sc, const BandPriority &prio) :
    stop_controller_(sc), // PRQA S 2528 2
    band_priority_(prio)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Band priority element " << hex << &band_priority_ << dec << " with priority : " << band_priority_.getBandPriority()) ;
}
    

namespace {
    inline long long toll(long l) { return l ; } // PRQA S 2134

    inline
    FtpProgressData::Time_ns nsec(const struct timespec &t) // PRQA S 2134
    { 
        return toll(t.tv_sec) * 1000000000 + t.tv_nsec ; // PRQA S 4400
    }

    inline 
    bool isOlder(FtpProgressData::Time_ns now, // PRQA S 2134
                 FtpProgressData::Time_ns f, 
                 FtpProgressData::Interval_ns v)
    {
        return now - f > v ;
    }
        
}


class remove_older { // PRQA S 2109
public:
    remove_older(FtpProgressData::Time_ns now, FtpProgressData::Interval_ns gap) : 
        now_(now), 
        gap_(gap)
    {}
     
    remove_older(const remove_older &) = default ;
    remove_older& operator= (const remove_older &) = default ;

        
    bool operator()(const FtpProgressData::Event & item) const // PRQA S 4020
    {
        return isOlder(now_, item.current_, gap_) ;
    }
        
private:
   FtpProgressData::Time_ns now_ ; 
   FtpProgressData::Interval_ns gap_ ;
} ;


FtpProgressData::Time_ns FtpProgressData::origin() noexcept
{
    static bool firsttime = true ;
    static FtpProgressData::Time_ns origin = 0 ;
    if (firsttime) {
        firsttime = false ;
        struct timespec now ; // PRQA S 4102
        ::clock_gettime (CLOCK_REALTIME, &now);
        origin = nsec(now) ;
    }

    return origin ;
}



FtpProgressData::Time_ns FtpProgressData::now() noexcept
{
    struct timespec now ; // PRQA S 4102
    ::clock_gettime (CLOCK_REALTIME, &now);
    return nsec(now) - origin() ;
}


void FtpProgressData::computeAndDelay(double partial) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB,"Delay calculation begins" );
    
    Time_ns _now = now() ; 
    ++counter_ ;
    if(! isRunning_)// if algo is not running fills memory_ with offset time 
    {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB,"Not Running" );
        memory_.emplace_front( _now, 0,  0, counter_);
        isRunning_ = true;
        return ;
    }
    else{                       // while algo is running fills memory_ with actual time
        ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB,"Running" );
        if (isOlder(_now, memory_.front().current_, 30000000)) { // 30ms // PRQA S 4400
            /*
            **Since memory_ is front-filled, memory_.front() is the newest element in memory_.
            **prev will keep track of  memory_.front() from now on.
            **Interval_ns machine is the amount of time the machine actually did work. 
            **Beside natural apporximation, machine could be intended as:
            **[Total_time - Total_Delay] evaluated since last element was pushed into memory_ .
            **After this evaluation, the new element is (front) pushed into memory_.
            */
            const Event &prev = memory_.front() ;
            Interval_ns machine = _now - prev.current_ - del_* (counter_-prev.counter_);
            memory_.emplace_front( _now, prev.del_partial_sum_ + machine,  partial, counter_);
        }
    }

    
    auto it_b = memory_.begin() ; 
    advance(it_b,2) ; // keeps at least 2 elements
    auto it = find_if(it_b, memory_.end(), remove_older(_now, windowLenght_)) ;
    memory_.erase(it, memory_.end());

    if( isOlder(_now , lastRun_, 500000000  )) // 0.5 s // PRQA S 4400
    {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Elements: " << memory_.size() ) ;
        Event &last = memory_.front() ;
        Event &first = memory_.back() ;
        if (&last != &first) {
            lastRun_ = _now ;
            BandPriority::Speed_bps givenBand = band_priority_.getBandLimit() ; // bps
            ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "Given Band: " << givenBand << " [bps] ") ;

            // at least two elemens are in memory
            Interval_ns tot_ns = last.current_ - first.current_ ;
            BandPriority::Speed_bps avgSpeed = 8E9 * (last.bytes_ - first.bytes_ )/tot_ns ; // bps NOSONAR - implicit conversion

            double ratio = avgSpeed/double(givenBand) ; // NOSONAR - implicit conversion
            // do not change delay if the difference is below 2%
            if((ratio > 1.02 )|| (ratio < 0.98)) // PRQA S 4400
            {
                Interval_ns tot_del = last.del_partial_sum_ - first.del_partial_sum_ ;
                long long deltac = last.counter_ - first.counter_ ;

                del_ = (ratio*tot_ns - tot_del)/deltac ; // NOSONAR - implicit conversion
                // rearrange sums
                last.del_partial_sum_ -= first.del_partial_sum_ ;
                first.del_partial_sum_ = 0; 
                ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB, "tot_ns: " << tot_ns << " tot_del: " << tot_del << " deltac: " << deltac ) ;
            
            }
 
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,"Delay: " << del_ << " [ns] ratio: " << (1-ratio)*100 << "% Avg speed: " << avgSpeed << "/" << givenBand << " [bps]");            
            del_ = min( max(Interval_ns(0),del_), Interval_ns(1000000000)) ; //PRQA S 3081, 4400

            ACS_CLASS_WRITE_DEBUG_TH(ACS_INS_VERB,"over " << memory_.size() << " elements" ) ;
        }
    }
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB,"Applying delay of " << del_ << " [ns]"  );            
    StopController::nanosleep(del_); // PRQA S 3000, 3010
}



int FtpProgressData::progress_callback(void* ptrtodata, // NOSONAR - callback
                                       double downloadTotal, 
                                       double downloadNow, 
                                       double uploadTotal,
                                       double uploadNow) 
{
    FtpProgressData* progressData = reinterpret_cast<FtpProgressData *>(ptrtodata); // NOSONAR - callback
    
    double partial = downloadNow + uploadNow ; // any - only one is valorized
    if (progressData->perfo_monitor_) {
        progressData->perfo_monitor_->progress(partial, downloadTotal + uploadTotal) ;
    }
    
    if (progressData->stop_controller_.haltImmediate()) {
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "halt level is IMMEDIATE");
        return -1;
    }
    
    
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB,"Current band priority: " << progressData->getBandPriority() ); 

    if ( (! progressData->isEnabled()) || ( 0 == progressData->getBandPriority() )) {
        return 0 ;
    }
    
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_HIG_VERB,"partial: " << partial );
    
    progressData->computeAndDelay(partial) ;
    
    return 0 ;
    
}

long FtpProgressData::getBandPriority() const
{
	return band_priority_.getBandPriority();
}
bool FtpProgressData::isEnabled() const // PRQA S 4214
{
    return band_priority_.isEnabled();
}

_ACS_END_NESTED_NAMESPACE
