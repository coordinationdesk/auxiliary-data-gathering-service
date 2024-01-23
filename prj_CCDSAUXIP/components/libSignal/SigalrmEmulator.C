// PRQA S 1050 EOF
/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libSignal $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/17 11:32:01  marpas
	adopting libException 5.x standards
	
	Revision 2.4  2013/01/25 11:16:20  marpas
	using Singleton pattern form libException
	coding best practices
	qa rules
	
	Revision 2.3  2012/11/06 17:44:33  marpas
	Quality improved, coverage test done
	
	Revision 2.2  2012/02/07 10:33:42  marpas
	removing compiler warnings
	
	Revision 2.1  2009/06/26 10:49:38  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.0  2006/02/28 08:29:41  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/05/06 07:39:24  marpas
	headers fixed
	
	Revision 1.1  2004/09/21 08:00:04  marfav
	Sigalrm emulator added to library
	

*/

#include <SigalrmEmulator.h>

#include <exException.h>
#include <Filterables.h>

#include <cerrno>


_ACS_BEGIN_NAMESPACE(acs)

using std::string;
using std::exception;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SigalrmEmulator)

SigalrmEmulator::SigalrmEmulator (): // PRQA S 2629
    Thread(),
    _handler(0),
    _dataHandler(0),
    _dataHandlerParameter(0),
    _timerValue(),
    _handlerMutex(),
    _valueMutex(),
    _delayMutex(),
    _timer(),
    _initialized(false),
    _delay()
{
}

SigalrmEmulator::~SigalrmEmulator () ACS_NOEXCEPT
{
    // PRQA S 4631 L1
	try {
		// Just to avoid "kill -9"
		Thread::stop(); // PRQA S 4631
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
    // PRQA L:L1
}

int
SigalrmEmulator::signal ( Handler p )
{
	// Thread safe implementation
	{
		acs::ThreadSafe::Lock guard(_handlerMutex); //automaticaly acquire the lock
		_handler = p;
	}
	return 0;
}

int
SigalrmEmulator::signal ( DataHandler p, void* data )
{
	// Thread safe implementation
	{
		acs::ThreadSafe::Lock guard(_handlerMutex); //automaticaly acquire the lock
		_dataHandler = p;
		_dataHandlerParameter = data;
	}
	return 0;
}


// Run is "protected" and cannot be used from outside this class
// The timer is started using the setitimer method

void
SigalrmEmulator::run ()
{

    {
       // set the delay with the "current" value of the timer
       acs::ThreadSafe::Lock guard(_delayMutex); //automaticaly acquire the lock
       _delay = (_timerValue.it_value.tv_sec * 1000)+ (_timerValue.it_value.tv_usec / 1000); // PRQA S 3000, 4400
    }

    // current value == 0 means timer disabled !!
    if (_delay > 0)
    {
	    // start the timer
	    _timer.start ();

	    // loop until there is a cancel request ....
	    while ( !requestedCancel() )
	    {
	        while ( (!requestedCancel()) && (_timer.stopElapsed() < _delay)) // PRQA S 3231, 4291, 4680, 4681, 4682, 4684
	        {
    		    // set the nanosleep period to 1/10 seconds
                // 100000000 nsec = 100 msec 
		        nanosleep (100000000); // PRQA S 4400
	        }
	        if (!requestedCancel())
	        {
		        Handler myHandler = 0;
			DataHandler myDataHandler = 0;
			void* myDataHandlerParameter = 0;

		        {
		            // avoid changing the handler during a call
		            acs::ThreadSafe::Lock guard(_handlerMutex); //automaticaly acquire the lock
		            myHandler = _handler;
			    myDataHandler = _dataHandler;
			    myDataHandlerParameter = _dataHandlerParameter;
		        }

		        if (0 != myHandler)
		        {
		            (*myHandler) (0);
		        }
			
			// Call also the other handler if requested
			if (0 != myDataHandler)
			{
			    (*myDataHandler) (myDataHandlerParameter);
			}

		        {
			        // now set the delay with the "interval" value
			        acs::ThreadSafe::Lock guard(_delayMutex); //automaticaly acquire the lock
			        _delay = (_timerValue.it_interval.tv_sec * 1000)+ (_timerValue.it_interval.tv_usec / 1000); // PRQA S 3000, 4400
		        }

		        // and restart the timer
		        _timer.start ();
	        }
	    }
    }
}

int
SigalrmEmulator::getitimer(int , struct itimerval *value)   // PRQA S 4020
{
	if (!value)
	{
		// there is a not valid pointer to the struct value
		// have to return -1 and set errno to EFAULT
		errno = EFAULT; //PRQA S 3224
		return -1;
	}

	itimerval it; // PRQA S 4102
	it.it_interval.tv_sec = it.it_interval.tv_usec = it.it_value.tv_sec = it.it_value.tv_usec = 0; // PRQA S 3902

	if (_initialized)
	{
		itimerval currentValue; // PRQA S 4102
		{
			acs::ThreadSafe::Lock guard(_valueMutex); //automaticaly acquire the lock
			currentValue= _timerValue;
		}

		it.it_interval.tv_sec = currentValue.it_interval.tv_sec;
		it.it_interval.tv_usec = currentValue.it_interval.tv_usec;
		Timer::Delay delay ; // PRQA S 4101

		{
			acs::ThreadSafe::Lock guard(_delayMutex); //automaticaly acquire the lock
			delay=_delay;
		}

		if (delay>0)
		{
			long d=delay - _timer.elapsed();    // PRQA S 3000, 3084
			if (d<=0) //worst case
			{
				// the timer has expired... avoid blocking it
				it.it_value.tv_usec= currentValue.it_interval.tv_usec;
				it.it_value.tv_sec = currentValue.it_interval.tv_sec;
			}
			else
			{   
                // d is in milliseconds
				it.it_value.tv_sec = d/1000;            // PRQA S 4400
				it.it_value.tv_usec =  (d%1000) * 1000; // PRQA S 4400
			}
		}
	}

	*value = it;
	return 0;
}


int
SigalrmEmulator::setitimer(int, const struct itimerval *value, struct itimerval *ovalue)    // PRQA S 4020, 4215
{
    // the which field is ignored
    // it is used only for system call prototype compatibility

    try
    {

	    // verify the value parameter
	    if (!value)
	    {
		    // there is a not valid pointer to the struct value
		    // have to return -1 and set errno to EFAULT
		    errno = EFAULT; //PRQA S 3224
		    return -1;
	    }

	    struct itimerval newValue = *value;

	    // stop the timer if it is running
	    this->stop();

	    // copy the old timeout value to ovalue
	    // if ovalue is a valid pointer
	    if (ovalue)
	    {
		    // the first parameter in the call will be ignored
		    getitimer (0, ovalue);
	    }


	    // set the new timer value
	    {
		    acs::ThreadSafe::Lock guard(_valueMutex); //automaticaly acquire the lock
		    _timerValue = newValue;
	    }

	    Timer::Delay delay = (newValue.it_value.tv_sec * 1000)+ (newValue.it_value.tv_usec / 1000);  // PRQA S 3000

	    // restart the thread using new parameters
	    // note that delay = 0 means "disable the timer"
	    // even if the interval value is not null !!
	    if (delay > 0 ) {
		    this->start();
        }

	    _initialized=true;
	    return 0;
    }
    catch (exception &e)
    {
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignature << "exception caught while starting the timer") ;
			ACS_LOG_NOTIFY_EX (e);
		ACS_CLASS_END_DEBUG
		return -1;
    }

}

void
SigalrmEmulator::stopTimer ()
{
	// Stop and join the thread if it is runnig
 	this -> stop();
}

_ACS_END_NAMESPACE

