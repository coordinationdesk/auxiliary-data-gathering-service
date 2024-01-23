// PRQA S 1050 EOF
/*

	Copyright 2014-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/07/15 11:44:40  nicvac
	Endline char handling.
	
	Revision 5.1  2014/05/20 12:16:13  marfav
	Addedd support for IPF Standalone app
	
	Revision 1.1  2014/05/14 14:32:55  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/


#include <IPFLogFmtFactory.h>

#include <Filterables.h>
#include <DateTime.h>
#include <RegExp.h>
#include <Application.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(IPFLogFmtFactory) 

enum Keys {all, hostN, procN, procV, task};

IPFLogFmtFactory::IPFLogFmtFactory() :
	exMsgFmtFactory(true, 	// emit PID
			true, 			// emit Time
			"",   			// start TAG
			"",				// end TAG
			" "				// separator
			),
	_procName(""),
	_procVersion(""),
	_taskName(""),
	_endLine('\n')
{
    IPFLogFmtFactory::emitTime(true) ;
}

IPFLogFmtFactory::IPFLogFmtFactory(bool pid, bool time, const string &start, const string &end, const string &sep) :
	exMsgFmtFactory(pid,  time,   start,   end,  sep),
	_procName(""),
	_procVersion(""),
	_taskName(""),
	_endLine('\n')
{
}

IPFLogFmtFactory::~IPFLogFmtFactory() throw()
{}


string IPFLogFmtFactory::text(const exFilterable &m) const 
{
	Lock lock(exostream::globalMutex()) ;

	string out ;

	//add time information + separator
	if (emitTime()) {
        ostringstream os ;
		DateTime().asISO8601(os,6) << separator() ; // PRQA S 4400
 		out += os.str() ;
    }
	
	//add specific application information : HostName + ProcName + ProcVersion
	out += Application::hostname() + separator() + procName();
	if (!procName().empty()) {
		out += separator() ;
    }
	out += procVersion() ;
	if (!procVersion().empty()) {
		out += separator() ;
    }
	
	//add PID information
	if (emitPid()) 
	{
		ostringstream pids ;
		pids << setw(10) << setfill('0') << ::getpid() ; // PRQA S 4400
		out += '[' + pids.str() + "]:" + separator();
	}
	
	//add specific message type information
	string messageType ;
	try
	{
		if (dynamic_cast<const exFWarning *>(&m)) { // PRQA S 3081
			messageType = "[W]" ;
    	}
    	else if (dynamic_cast<const exFProgress *>(&m)) { // PRQA S 3081
		  messageType = "[P]" ;
    	}
    	else if (dynamic_cast<const exFError *>(&m)) { // PRQA S 3081
			messageType = "[E]" ;
		}
		else if (dynamic_cast<const exFDebug *>(&m)) { // PRQA S 3081
			messageType = "[D]" ;
    	}
    	else if (dynamic_cast<const exFMessage *>(&m)) { // PRQA S 3081
			messageType = "[I]" ;
    	}
    	else {
			messageType = "[D]" ;
    	}
	}
	catch (exception &e)
	{
		messageType = "Error IPFLogFmtFactory level exception)" ;
	}
	
	if (not messageType.empty()) {
		out += messageType + separator() ;
	}

    //Free text begins with the task name
    out += taskName() ;
    if (!taskName().empty()) {
        out += ':' + separator() ;
    }

	string msg = m.message() ;
	
	// probably not the best way but does the job.
	size_t last = msg.length()-1 ; // PRQA S 3084
	if (msg[last] == '\n') {
		msg[last] = (not separator().empty()) ?  separator()[0] : ' ' ; // PRQA S 3380
    }
		
	//If configured, remove all '\n' from message.
	if ( _endLine != '\n' ) {
		std::replace( msg.begin(), msg.end(), '\n', _endLine);
	}

	out += msg ;
	
	return out ;
}

_ACS_END_NAMESPACE

