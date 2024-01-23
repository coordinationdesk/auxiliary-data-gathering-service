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
 *  Revision 5.9  2014/09/10 16:06:31  francesco.avanzi
 *  now also checking if Band Shaping is enabled
 *
 *  Revision 5.8  2014/02/25 09:35:50  francesco.avanzi
 *  qac++analisys run
 *
 *  Revision 5.7  2014/02/24 20:49:07  marpas
 *  perfo and progress data rationalization
 *
 *  Revision 5.6  2014/02/21 16:47:54  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.5  2014/02/20 14:14:04  francesco.avanzi
 *  implementation is over, Test is executed
 *
 *  Revision 5.4  2014/02/14 13:54:32  marpas
 *  attempt to refactor band shaping mechanism
 *
 *  Revision 5.3  2013/10/23 17:28:11  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/10/22 17:31:51  marpas
 *  work in progress
 *
 *  Revision 5.1  2013/06/18 12:54:14  marpas
 *  debug macros rationalized
 *
 *  Revision 5.0  2013/06/06 18:08:21  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.7  2013/05/29 10:53:59  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.6  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.5  2013/02/28 13:30:31  marpas
 *  libMessage dependencies removed
 *
 *  Revision 2.4  2012/11/29 16:28:20  micmaz
 *  quality tool warnings removed on destructors.
 *
 *  Revision 2.3  2012/02/07 10:25:47  micmaz
 *  removed warnings .
 *
 *  Revision 2.2  2012/01/20 16:06:58  micmaz
 *  dtor made virtual.
 *
 *  Revision 2.1  2012/01/16 10:18:39  micmaz
 *  IN PROGRESS - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.0  2011/10/18 14:43:51  marpas
 *  new statistics adopted
 *
 *  Revision 1.6  2011/05/10 17:21:30  micmaz
 *  corrected bug of ProgressData callback: destroyed while working.
 *
 *  Revision 1.5  2011/05/09 16:24:46  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/15 18:01:33  micmaz
 *  work in progress
 *
 *  Revision 1.3  2011/04/11 12:25:06  micmaz
 *  work in progress
 *
 *  Revision 1.2  2011/04/08 15:37:40  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/04/05 17:15:09  micmaz
 *  *** empty log message ***
 *
 *
 *
 *
 */

#ifndef FtpProgressData_H_
#define FtpProgressData_H_

#include <FtpPerformance.h>
#include <StopController.h>
#include <list>
#include <utility>
#include <algorithm>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;

class BandPriority ; 
class FtpPerformance ; 

class FtpProgressData { // PRQA S 2109
public:

    FtpProgressData(const StopController &sc, const BandPriority & ) ;
    
    FtpProgressData(const FtpProgressData & ) = delete ;
    FtpProgressData & operator= (const FtpProgressData & ) = delete ;
    
    ~FtpProgressData() = default ;


    void setPerfMonitor(FtpPerformance *v) noexcept { perfo_monitor_ = v ;  }
	long getBandPriority()const ;
    bool isEnabled()const;
    //
    //Callbacks section
    //
    static int progress_callback(void* ptrtodata, // NOSONAR - callback
                                 double downloadTotal, 
                                 double downloadNow, 
                                 double uploadTotal,
                                 double uploadNow) ;
    
    using Time_ns = long long  ;
    using Interval_ns = long long  ;

    struct Event { 
        Event() = default ;
        Event(Time_ns c, Interval_ns p , double b, long long i) :current_(c), del_partial_sum_(p), bytes_(b), counter_(i) {}
        Time_ns      current_ = 0 ; // the time when the event occurred                 
        Interval_ns  del_partial_sum_ = 0 ; // the partial sum of delay up to the Event 
        double       bytes_ = 0 ; // the byte transferred in this event                 
        long long    counter_ = 0 ; // progressive id of the event                      
    } ;
    
    using EventContainer = std::list <Event >  ; 
    
private:
    void computeAndDelay(double partial) ;
    static Time_ns origin() noexcept ;
    static Time_ns now() noexcept ;

    /**
     * Reference to a Worker object.
     *
     * The default progress callback checks the current state of this referenced StopController, if a stop request
     * has been properly issued, then the default progress callback detects it by
     * inspecting worker's state and aborts the transfer.
     */
    const StopController &stop_controller_ ;
    const BandPriority &band_priority_ ;
    
    EventContainer memory_ = {} ; //keeps track of velocities.
    Interval_ns windowLenght_ = 500000000 ;  // 0.5 s interval in which average speed is calculated 
    Interval_ns del_ = 0 ;       //Applied delay
    Time_ns lastRun_ = 0 ;       //keeps track of last delay() call
    bool isRunning_= false ; //keeps track whether the data-transfert is running or not

    FtpPerformance *perfo_monitor_ = nullptr ;
    long long counter_ = 0 ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(FtpProgressData) ;
};


_ACS_END_NESTED_NAMESPACE

#endif /* FtpProgressData_H_ */
