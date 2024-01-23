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

	$Prod: A.C.S. Recv timeout using pipe trick $

	$Id$

	$Author$
    
    $Log$
    Revision 2.6  2013/01/25 11:16:19  marpas
    using Singleton pattern form libException
    coding best practices
    qa rules

    Revision 2.5  2012/11/06 17:44:33  marpas
    Quality improved, coverage test done

    Revision 2.4  2012/10/29 14:42:54  marpas
    robusteness, quality, coverage

    Revision 2.3  2012/10/25 15:58:54  marpas
    ctor made explicit

    Revision 2.2  2012/10/24 16:32:33  marpas
    new ctor to pass time slices for checking thread cancellation
    default behaviour uses 1/10th of the timeout for the timeslice
    the recv_to do not throws exceptions

    Revision 2.1  2012/10/24 15:58:37  marpas
    RecvTimeoutPipeTrick class and test added



*/

#ifndef _RecvTimeoutPipeTrick_H_
#define _RecvTimeoutPipeTrick_H_

#include <acs_c++config.hpp>

#include <Timer.h>
#include <Thread.h>


_ACS_BEGIN_NAMESPACE(acs)

/** 
 *\brief class RecvTimeoutPipeTrick 
 * 
 *
 * The RecvTimeoutPipeTrick allow a user to call a recv_to method over a file descriptor.
 * internally it uses a select in blocking mode, but allow a trhead to interrupt the select 
 * after a timeout since the starting of the read.
 * 
 */
class RecvTimeoutPipeTrick : private Thread     // PRQA S 2109, 2193
{
public:
    explicit RecvTimeoutPipeTrick(Timer::Delay ms_timeout) ;
    RecvTimeoutPipeTrick(Timer::Delay ms_timeout, Timer::Delay ms_slices ) ;
    virtual ~RecvTimeoutPipeTrick() ACS_NOEXCEPT ;
    
    ssize_t recv_to(int fd, void *buf, size_t len) ACS_NOEXCEPT ;
    
private:
    // declared but not implemented
    RecvTimeoutPipeTrick() = delete ; // PRQA S 2038 5
    RecvTimeoutPipeTrick(const RecvTimeoutPipeTrick &) = delete ;
    RecvTimeoutPipeTrick & operator= (const RecvTimeoutPipeTrick &) = delete ;
    RecvTimeoutPipeTrick(RecvTimeoutPipeTrick &&) = delete ;
    RecvTimeoutPipeTrick & operator= (RecvTimeoutPipeTrick &&) = delete ;
    
    void openPipe() ;
    void closePipe() ACS_NOEXCEPT ;
	virtual void run() ;
    
private:
    static const int ReadEnd = 0 ;
    static const int WriteEnd = 1 ;

private:

    Timer::Delay    timeout_ ;
    Timer::Delay    slices_timeout_ ;
    int             pipefd_[2] ;        // PRQA S 4403
    Timer           timer_ ;
    ACS_CLASS_DECLARE_DEBUG_LEVEL(RecvTimeoutPipeTrick) ;
};



_ACS_END_NAMESPACE

#endif //_Signal_H_
