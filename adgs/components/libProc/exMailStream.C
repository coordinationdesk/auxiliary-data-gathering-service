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

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.14  2014/12/17 11:39:04  marfav
	Fixed management of FD closure
	
	Revision 2.13  2013/11/22 20:18:31  marpas
	coding best practices applied
	
	Revision 2.12  2013/09/27 14:06:59  marpas
	streams have names
	
	Revision 2.11  2013/07/25 14:07:17  marfav
	Delay removed
	
	Revision 2.10  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.9  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.8  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.7  2011/03/18 10:51:40  marpas
	scope fixed
	
	Revision 2.6  2011/03/01 11:52:55  marpas
	GCC 4.4.x support
	
	Revision 2.5  2009/07/14 12:40:09  matteo.airoldi
	Added include unistd to compile with RHEL3
	
	Revision 2.4  2009/06/26 10:46:49  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.3  2009/04/03 15:13:03  marpas
	check for no to address ...
	
	Revision 2.2  2009/04/03 10:47:56  marpas
	subjectPrefix method added
	
	Revision 2.1  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	
	
*/

#include "exMailStream.h"
#include <SpawnProc.h>
#include <File.h>
#include <Timer.h>
#include <unistd.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


//
//
// exHandler
//
//
exMailStream::exMailStream(const std::string &from) :
    exBaseStream("exMailStream"),
	_from(from),
    _to(),
	_tmp(0),
    _subjectPrefix()
{
}

exMailStream::exMailStream(exostream *parent, const std::string &from) :
	exBaseStream("exMailStream",parent),
	_from(from) ,
    _to(),
	_tmp(0),
    _subjectPrefix()
{
}

exMailStream::exMailStream(const vector<exostream *> &parents, const std::string &from) :
	exBaseStream("exMailStream",parents),
	_from(from),
    _to(),
	_tmp(0),
    _subjectPrefix()
{
}

exMailStream::~exMailStream() throw()
{
    // PRQA S 4631 L1
	destroying() ;
	delete _tmp ;
    // PRQA L:L1
}



void exMailStream::sendmail(const string &subject) throw() 
{
	// prepare to send mail:
	if (!_to.empty()) {
		// the executable to be launched
		SpawnProc proc("mail") ;

		// next the subject
		int args = 0 ;
		proc.args()[args++] = WaitPid::ProcArg(string("-s") + "\"" + (_subjectPrefix.length() ? _subjectPrefix + " " : string("")) + subject + "\"") ; // PRQA S 3000, 3054, 3081, 3380, 3385

		// every address to contact // PRQA S 4238, 4244 2
		for (size_t i = 0; i< _to.size(); ++i) {
			proc.args()[args++] = WaitPid::ProcArg(_to[i]) ;    // PRQA S 3000, 3081
        }

		// if there is a From
		if (not _from.empty()) {
			proc.args()[args++] = WaitPid::ProcArg("--") ;      // PRQA S 3000, 3081 3
			proc.args()[args++] = WaitPid::ProcArg("-f") ;
			proc.args()[args++] = WaitPid::ProcArg(_from) ;
		}

		// now the pipes
		// 0 to be given as input to the process
		int infiledes[2] ;  // PRQA S 4403
		pipe(infiledes) ;
		proc.setInDescriptor(infiledes[0]) ;

		{
			// the other pipe end to write the mail body
            // the file descriptor will not be closed automatically by ofdstream
			File::ofdstream output(infiledes[1], false) ;
			// run the process
			proc.spawn() ;

			// close the useless end 
			if (0 != ::close(infiledes[0]) )
				ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));

			// write the mail body
			output << _tmp->str() << "\n" ;
			// here the output goes out of scope - so the process will notice the EOF and will send the mail
		}

		// wait for process completion
		while(proc.isRunning()) {
			proc.waitProcCompletion(); 
			// Timer::delay(50) ;  // PRQA S 4400
        }

		// close the used end 
		if (0 != ::close(infiledes[1]))
			ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno));
	}
	// remopve the stream used in order to create another one when needed 
	// see method stream() 
	delete _tmp ;
	_tmp = 0 ;
}

bool exMailStream::notifyException(const std::exception &x) throw ()
{
	// will write into a new stream (creating a new one)
	bool ret = exBaseStream::notifyException(x) ;
	
	// send the mail
	sendmail("Exception") ;
	
	return ret ;
}


void exMailStream::notifyFilterable(exFilterable &s) 
{
	// will write into a new stream (creating a new one)
	exBaseStream::notifyFilterable(s) ;

	// send the mail
	sendmail("Message") ;
}  

void exMailStream::addTo(const std::string &to) 
{
	vector<string >::iterator tbi = find(_to.begin(), _to.end(), to) ;
	if (tbi == _to.end()) {	_to.push_back(to) ; }
}

void exMailStream::addTo(const std::vector<std::string> &to) 
{
	for(size_t i=0; i < to.size(); i++) {
		addTo(to[i]) ;
    }
}


std::ostream &exMailStream::stream() 
{
	if (!_tmp) {
		_tmp = new ostringstream ;
	}
    
	return *_tmp ;
		
}

void exMailStream::subjectPrefix(const string &sp) // PRQA S 4121
{
	_subjectPrefix = sp ;
}


_ACS_END_NAMESPACE
