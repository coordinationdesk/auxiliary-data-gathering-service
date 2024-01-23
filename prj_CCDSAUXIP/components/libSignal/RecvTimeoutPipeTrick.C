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
    Revision 5.2  2016/04/14 09:56:33  marpas
    Thread does not inheriths any longer from ThreadSafe

    Revision 5.1  2015/05/08 09:17:10  davide.tiriticco
    unistd inlude added

    Revision 5.0  2013/06/17 11:32:01  marpas
    adopting libException 5.x standards

    Revision 2.5  2013/01/25 11:16:19  marpas
    using Singleton pattern form libException
    coding best practices
    qa rules

    Revision 2.4  2012/11/06 17:44:33  marpas
    Quality improved, coverage test done

    Revision 2.3  2012/10/29 14:42:54  marpas
    robusteness, quality, coverage

    Revision 2.2  2012/10/24 16:32:33  marpas
    new ctor to pass time slices for checking thread cancellation
    default behaviour uses 1/10th of the timeout for the timeslice
    the recv_to do not throws exceptions

    Revision 2.1  2012/10/24 15:58:37  marpas
    RecvTimeoutPipeTrick class and test added



*/

#include <RecvTimeoutPipeTrick.h>
#include <Filterables.h>
#include <cstring>
#include <sys/socket.h> // PRQA S 1013
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RecvTimeoutPipeTrick) ;

RecvTimeoutPipeTrick::RecvTimeoutPipeTrick(Timer::Delay ms_timeout) : 
    Thread(),
    timeout_(ms_timeout),
    slices_timeout_(std::max(ms_timeout/10,Timer::Delay(1))), // PRQA S 3081, 3084, 4400
    pipefd_(),
    timer_()
{
    openPipe() ;
}


RecvTimeoutPipeTrick::RecvTimeoutPipeTrick(Timer::Delay ms_timeout, Timer::Delay ms_slices) : 
    Thread(),
    timeout_(ms_timeout),
    slices_timeout_(ms_slices),
    pipefd_(),
    timer_()
{
    openPipe() ;
}



RecvTimeoutPipeTrick::~RecvTimeoutPipeTrick() ACS_NOEXCEPT 
{
    // PRQA S 4631 L1
    try {
        Thread::stop() ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    closePipe() ;
    // PRQA L:L1
}


void RecvTimeoutPipeTrick::openPipe() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    int res = pipe(pipefd_) ; // PRQA S 3840 # it is the correct usage as per signature
    ACS_COND_THROW(res < 0, exIOException("Cannot create pipe", errno)) ; //PRQA S 3224
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "pipefd_[ReadEnd] = " << pipefd_[ReadEnd]
            << " pipefd_[WriteEnd] = " << pipefd_[WriteEnd]) ;
    
  
}


void RecvTimeoutPipeTrick::closePipe() ACS_NOEXCEPT 
{
    ::close(pipefd_[ReadEnd]) ;
    ::close(pipefd_[WriteEnd]) ;
    
    pipefd_[ReadEnd] = 0 ;
    pipefd_[WriteEnd] = 0 ;
    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "pipefd_[ReadEnd] = " << pipefd_[ReadEnd]
            << " pipefd_[WriteEnd] = " << pipefd_[WriteEnd]) ;
}


void  RecvTimeoutPipeTrick::run() 
{
    timer_.start() ;
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    const unsigned char byte = 0x7f ; 
    do { // PRQA S 4233
        if (timer_.isOlder(timeout_)) {
            ::write(pipefd_[WriteEnd], &byte, sizeof(byte)) ; 
            timer_.start() ;
        }
    } while (not timeoutOrCancel(slices_timeout_) ) ;
}

// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast - FD_XXX
#pragma GCC diagnostic ignored "-Wold-style-cast"
ssize_t RecvTimeoutPipeTrick::recv_to(int fd, void *buf, size_t len) ACS_NOEXCEPT // PRQA S 4020, 4215
{
    try {
        if (not running()) { // timout thread is not running 
            start() ; // start it
        }
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        return -3 ; // PRQA S 4400
    }
    
    fd_set rfds ;                       // PRQA S 4102
    FD_ZERO(&rfds) ;                    // PRQA S 4092, 4101, 4107
    // read end of pipe
    FD_SET(pipefd_[ReadEnd], &rfds ) ;  // PRQA S 2844, 3003, 3080, 3081
    // real fd
    FD_SET(fd, &rfds ) ;                // PRQA S 2844, 3003, 3080, 3081
    
    int nfds = std::max(fd,pipefd_[ReadEnd]) +1 ;
    // reset timer 
    timer_.start() ;

    // now the blocking select 
    int sr = select(nfds,&rfds, 0, 0, 0) ; // note no timeout
    
    if (sr < 0) { // select error 
        return ssize_t(sr) ; // just return it  // PRQA S 3081
    }

    ssize_t rr = -2 ; // assume recv on fd was never called
    if (FD_ISSET(fd, &rfds)) { // data available 
        rr = recv(fd, buf, len, 0) ;    // PRQA S 3010
        // reset timer 
        timer_.start() ;
    }
    
    bool to = false ; // assume no timeout

    if (FD_ISSET(pipefd_[ReadEnd], &rfds)) { // got timeout 
        // consume the data ;
        char buf_[1024] ; // PRQA S 4403
        ::read(pipefd_[ReadEnd], buf_, sizeof(buf_)) ; // PRQA S 3802, 3840 # really do not give a ....
        to = true ;
    }
        
    if ( -2 == rr ) { // PRQA S 4400
        // here if no data was read from fd
        // to means timeout - else there was some other problem
        return to ? -2L : -1L ; // PRQA S 3382, 4400
    }
    
    return rr ; 
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"


_ACS_END_NAMESPACE

