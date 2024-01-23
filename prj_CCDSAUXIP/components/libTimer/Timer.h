// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Timer Library $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2013/04/19 11:44:49  marpas
	documentation updated
	
	Revision 2.6  2013/02/22 10:34:06  marpas
	Timer does not lonh inherits virtually form ThreadSafe
	Ita has an internal mutex instead.
	
	Revision 2.5  2012/11/27 13:16:31  marpas
	qa rules
	
	Revision 2.4  2012/11/27 12:24:53  marpas
	qa rules
	
	Revision 2.3  2012/02/09 14:14:16  marpas
	refactoring
	
	Revision 2.2  2009/10/06 12:13:40  marpas
	interface improved
	
	Revision 2.1  2009/05/15 15:14:34  marpas
	rules fixing
	
	Revision 2.0  2006/02/28 08:28:40  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/05/06 08:39:53  marpas
	headers fixed
	
	Revision 1.7  2004/08/30 14:13:54  marpas
	randomDelay method added
	Note that according to the nanosleep man, resolution is 10msec on Linux/i386
	
	Revision 1.6  2004/04/23 09:10:41  marpas
	delay method is static now
	
	Revision 1.5  2004/04/14 09:43:46  marpas
	Thread safe class used
	
	Revision 1.4  2003/07/11 12:57:02  marpas
	exException::clone implemented
	
	Revision 1.3  2003/06/11 14:39:52  marpas
	isOlder method added, architectural slightly changed
	
	Revision 1.2  2003/01/15 17:08:37  marpas
	thread safe
	
	Revision 1.1.1.1  2002/10/04 16:26:05  paoscu
	Import libTimer
	

*/

#ifndef _Timer_H_
#define _Timer_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <sys/time.h>   // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

class Timer {   // PRQA S 2109, 2153

public:
	/*! class DateTimeMisalignement declaration */
	exDECLARE_EXCEPTION(DateTimeMisalignement,exException) ;        // PRQA S 2131, 2502

public:
	typedef unsigned long Delay ; // always in [msec]
	Timer() ;                                                       // PRQA S 1703
	virtual ~Timer() throw() ;                                       // PRQA S 1703

	Timer(const Timer &) ;
	Timer &operator=(const Timer &) ;

	static void delay(Delay msec) ; // uses select
	static void randomDelay(Delay maxMsec, Delay minMsec=0) ;
	void start() ;
    /*! this method does not actually stops the time flow, but just freeze the read at this time.
        From now on, any call to elaplsed will return the difference from the stop and the previous start.
        So, if you are interested in time since the start up to this moment, remember to call stop each time 
        before calling elapsed to read the difference.
     */
	void stop() ;
    /*!
        Returns the amount of ms (Delay type) elapsed from the last start() to the last stop()
        Note that if the last stop does not occurred since the last start, this method calls stop().
    */
	Delay elapsed() ; // [msec]
    /*!
        Returns the amount of ms (Delay type) elapsed from the creation.
        IT IS FALSE - CURRENTLY IT BEHAVES EXACTLY AS THE SEQUENCE stop() , elapsed() ;
    */
	Delay __attribute__((deprecated)) life() ; // [msec]

    /*!
        Returns the amount of ms (Delay type) elapsed from the last start, calling the stop first
    */
	Delay stopElapsed() { stop(); return elapsed(); } // [msec]
	
	// note: the function can check if a date misalignement occurred (start time > stop time)
	// and in this case it signals the problem emitting an exception message on excerr
	// but it will return true, in order to signal the application its time to perform the action.
	// Moreover, under these circumstances, it will set start time equal to stop time.
	bool isOlder(Delay) ;
	bool running() const throw() ;
	
protected:
	void stopLife() ;

#ifdef TESTTIMER
public:
#else
private:
#endif
	struct timeval _start ; 
    struct timeval _stop ;
    struct timeval _life ;
	struct timezone	_zone ;	
	bool _running ;
    Mutex _mutex ;
	static bool _srandInitialized ;
} ;



class RTTimer {
public:
	RTTimer() ;                                                       // PRQA S 1703
	virtual ~RTTimer() throw() ;                                       // PRQA S 1703

	RTTimer(const RTTimer &) ;
	RTTimer &operator=(const RTTimer &) ;

	void start() ;
    /*! this method does not actually stops the time flow, but just freeze the read at this time.
        From now on, any call to elaplsed will return the difference from the stop and the previous start.
        So, if you are interested in time since the start up to this moment, remember to call stop each time 
        before calling elapsed to read the difference.
     */
	void stop() ;
    /*!
        Returns in sec the elapsed time from the last start() to the subsequent stop()
        Note that if the last stop does not occurred since the last start, this method calls stop().
    */
	long double elapsed() ; // [sec]

private:
	struct timespec _start ; 
    struct timespec _stop ;
	bool _running ;
    Mutex _mutex ;
} ;

_ACS_END_NAMESPACE

#endif // _Timer_H_
