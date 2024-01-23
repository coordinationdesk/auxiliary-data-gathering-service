// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.12  2015/01/20 14:18:27  marfav
	checking exit code of close syscall
	
	Revision 2.11  2014/12/17 11:39:04  marfav
	Fixed management of FD closure
	
	Revision 2.10  2014/04/02 11:32:25  marpas
	new throwing macros adopted
	if ifucstream (uncompress) does not find the executable, will try with gunzip
	[io]fucstream classes refactorized to not deal directly with pointers (auto_ptr used)
	SpawnProc ctor has a new flag defaulted to true to resolve symbolic link on commands.
	CmdIORedir class uses the above flag to false (because compress and uncompress commands are the same executable, but their behaviour depends on the argv[0] i.e. the summoned name)
	
	Revision 2.9  2013/11/22 20:18:32  marpas
	coding best practices applied
	
	Revision 2.8  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.7  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.6  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.5  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.4  2006/05/22 14:01:54  clanas
	added unistd for RHEL 3.x gcc 3.2.x
	
	Revision 2.3  2006/05/19 18:58:48  marpas
	non  blocking i/o - SpawnProc used
	
	Revision 2.2  2006/05/19 09:47:20  marpas
	files are always read from beginning
	
	Revision 2.1  2006/05/18 15:11:46  marpas
	ftailstream class family added (implemented only for input - iftailstream)
	
	

*/
#include <unistd.h>
#include <ftailstream.h>
#include <File.h>
#include <ctime>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(iftailstream)
const bool iftailstream::defaultCerrOnCout ;

iftailstream::iftailstream() : // PRQA S 4206, 4051 5
    _path(),
	_cerrOnCout(defaultCerrOnCout),
	_inb(0),
    _filedes(),
	_proc(0) 
{
    resetFDs();
}

iftailstream::~iftailstream() 
{
	close() ;
}

iftailstream::iftailstream(const std::string &path, bool cerrOnCout) : // PRQA S 4206, 4207, 4051 5
	_path(path),
	_cerrOnCout(cerrOnCout),
	_inb(0),
    _filedes(),    
	_proc(0) 
{
    resetFDs();
	open(_path,_cerrOnCout) ;
}

void iftailstream::resetFDs() throw()
{
    _filedes[0] = -1;
    _filedes[1] = -1;
}

void iftailstream::close() throw()
{
	delete _inb ;
	_inb = 0 ;
	delete _proc ;
	_proc = 0 ;

    if (-1 != _filedes[0]){
    	if (0 != ::close(_filedes[0]) ) {
		    ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
        }
    }
        
    if (-1 != _filedes[1]) {
    	if ( 0 != ::close(_filedes[1]) ) {
		    ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
        }
    }

    resetFDs();
}

void iftailstream::open(const std::string &path, bool cerrOnCout) 
{
	close() ;
	pipe(_filedes) ;

	_path = path ;
	_proc = new SpawnProc("tail") ;
	_proc->args()[0] = WaitPid::ProcArg("--retry") ;            // PRQA S 3081, 4400 4
	_proc->args()[1] = WaitPid::ProcArg("--follow=name") ;
	_proc->args()[2] = WaitPid::ProcArg("--bytes=+0") ;
	_proc->args()[3] = WaitPid::ProcArg(path) ;
	_proc->setOutDescriptor(_filedes[1]) ;
	if (cerrOnCout) {
		_proc->setErrDescriptor(_filedes[1]) ;
    }

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "spawning tail");

	try {
		_proc->spawn() ;
	}
	catch(exception &e) {
		delete _proc ;
		_proc = 0 ;
		if (0 != ::close(_filedes[0]) ) {
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
        }
		if (0 != ::close(_filedes[1]) ) {
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
        }
        resetFDs();
		ACS_THROW(exFileOpenException(e,"Cannot run tail on " + _path)) ; // PRQA S 3081
	}
}


// PRQA S 1040 L2
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"


bool iftailstream::hasData() const  // PRQA S 4020
{
	if (!_proc) { return false ; }
	if (_inb && !_inb->eof()) {	return true ; }
	fd_set rfds;                        // PRQA S 4102
	FD_ZERO(&rfds);                     // PRQA S 4092, 4101, 4107
	FD_SET(_filedes[0], &rfds);         // PRQA S 3003, 3080, 3081
	struct timeval tv;                   // PRQA S 4102
 
	/* Wait up to 1 msec */
	tv.tv_sec = 0;
	tv.tv_usec = 1000;                  // PRQA S 4400

	int ret = select(_filedes[0]+1, &rfds, 0, 0, &tv) ;
	if (ret < 0) {
		return false ;
    }
	return FD_ISSET(_filedes[0], &rfds) ;
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"
// PRQA L:L2


string & iftailstream::getline(string &gline) // PRQA S 4020
{
	if (_inb) {
		std::getline(*_inb,gline) ;
		if (!_inb->eof()) {
			return gline ;  // PRQA S 4028
		}
		delete _inb ;
		_inb = 0 ;
		return gline ;      // PRQA S 4028
	}
	gline = "" ;
	if (_proc) {

		char line[16384] ;  // PRQA S 4403
		line[0] = 0 ;

		int read = ::read(_filedes[0], line, sizeof(line)-1) ;  // PRQA S 3010, 3084
		if (read > 0) {
			line[read] = 0 ;
			_inb = new istringstream(string(line)) ; // PRQA S 3081
			return getline(gline) ;
		}
	}
	return gline ;          // PRQA S 4028
}

_ACS_END_NAMESPACE
