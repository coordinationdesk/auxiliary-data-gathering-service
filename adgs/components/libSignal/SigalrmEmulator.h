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

	$Prod: A.C.S. libSignal $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/01/25 11:16:20  marpas
	using Singleton pattern form libException
	coding best practices
	qa rules
	
	Revision 2.2  2011/01/24 15:31:19  marpas
	Mutex.h does not longer exixts
	
	Revision 2.1  2009/06/26 10:49:39  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.0  2006/02/28 08:29:41  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/05/06 07:39:24  marpas
	headers fixed
	
	Revision 1.2  2004/09/21 08:15:09  marfav
	SigalrmEmulator moved in namespace acs
	
	Revision 1.1  2004/09/21 08:00:04  marfav
	Sigalrm emulator added to library
	

*/

#ifndef _SIGALRMEMULATOR_H_
#define _SIGALRMEMULATOR_H_

#include <acs_c++config.hpp>

#include <Timer.h>
#include <Thread.h>

#include <sys/time.h> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)

class SigalrmEmulator : private Thread // PRQA S 2109, 2193
{
public:
	SigalrmEmulator ();
	virtual ~SigalrmEmulator () ACS_NOEXCEPT ;

	typedef void (*Handler) (int);
	typedef void (*DataHandler) (void*);

	int signal ( Handler );
	int signal ( DataHandler, void* data);

	int setitimer(int which /* unused */, const struct itimerval *value, struct itimerval *ovalue); // PRQA S 2502 2
	int getitimer(int which /* unused */, struct itimerval *value);
	void stopTimer();

protected:
	virtual void run ();

private:
	SigalrmEmulator (const SigalrmEmulator&) = delete ;
	SigalrmEmulator& operator = (const SigalrmEmulator&) = delete ;
	SigalrmEmulator (SigalrmEmulator&&) = delete ; // PRQA S 2038 2 
	SigalrmEmulator& operator = (SigalrmEmulator&&) = delete ;

private:
    Handler _handler ;
    DataHandler _dataHandler ;
    void* _dataHandlerParameter ;
	struct itimerval _timerValue;
	Mutex _handlerMutex;
	Mutex _valueMutex;
	Mutex _delayMutex;
	Timer _timer;
	bool _initialized;
	Timer::Delay _delay;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SigalrmEmulator) ;
};

_ACS_END_NAMESPACE

#endif // _SIGALRMEMULATOR_H_
