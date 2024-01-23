// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: ADGS Standalone App $

*/

#include <ADGSLogFmtFactory.h>

#include <Filterables.h>
#include <exFMCFilterables.h>
#include <DateTime.h>

#include <Application.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ADGSLogFmtFactory) 


ADGSLogFmtFactory::ADGSLogFmtFactory() :
	exMsgFmtFactory()
{
	ADGSLogFmtFactory::emitTime(true) ;
}

ADGSLogFmtFactory::ADGSLogFmtFactory(bool pid, bool time, const string &start, const string &end, const string &sep) :
	exMsgFmtFactory(pid,  time,   start,   end,  sep)
{
}

ADGSLogFmtFactory::~ADGSLogFmtFactory() throw()
{}

string &ADGSLogFmtFactory::format(string &out, const string &messageType, const string &message) const 
{
	out += startTag() ;

	if (emitTime()) {
		out += DateTime().asISO8601(6) + separator() ; // PRQA S 4400
	}

	out += Application::hostname() + separator() ;

	out += appName() ;
	if (!appName().empty()) {
		out += separator() ;
    }

	if (emitPid()) {
		ostringstream pids ;
		pids << '[' << ::getpid() << ']' << separator() ;
		out += pids.str() ;
	}

	out += messageType + separator();

	string msg = message ;
	
	// probably not the best way but does the job.
	size_t last = msg.length()-1 ; // PRQA S 3084
	if (msg[last] == '\n') {
		msg.erase(last) ;
    }
		
	out += msg ;
	
	out += endTag() ;
	
	
	return out ; // PRQA S 4028
}

string ADGSLogFmtFactory::text(const exFilterable &m) const  // PRQA S 4020
{

	Lock lock(exostream::globalMutex()) ;

	string messageType = "UnkLevel" ;

	try
	{	
		// Note since there is a hierarchy of filerable classes dynamc casts 
        // should be tried in a precise (hyerarchical) order
		// 
        if( dynamic_cast< mc::exFMCCritical const * >(&m) ) { // PRQA S 3081
            messageType = "MC_Critical" ;
        }
		else if( dynamic_cast< mc::exFMCError const * >(&m) ) { // PRQA S 3081
            messageType = "MC_Error" ;
        }
		else if( dynamic_cast< mc::exFMCWarning const * >(&m) ) { // PRQA S 3081
            messageType = "MC_Warning" ;
        }
		else if( dynamic_cast< mc::exFMCInfo const * >(&m) ) { // PRQA S 3081
            messageType = "MC_Info" ;
        }
		else if( dynamic_cast< exFDebug const * >(&m) ) { // PRQA S 3081
            messageType = "Debug" ;
        }
		else if( dynamic_cast< exFError const * >(&m) ) { // PRQA S 3081
            messageType = "Error" ; 
        }
		else if( dynamic_cast< exFWarning const * >(&m) ) { // PRQA S 3081
            messageType = "Warning" ; 
        }
		else if( dynamic_cast< exFOpEventAlert const * >(&m) ) { // PRQA S 3081
            messageType = "Alert" ; 
        } // Note son of exFEvent
		else if( dynamic_cast< exFEvent const * >(&m) ) { // PRQA S 3081
            messageType = "Notice" ; 
        }
		else if( dynamic_cast< exFOpAlert const * >(&m) ) {  // PRQA S 3081
            messageType = "Alert" ; 
        }
		
		// Note since exFMessage is the parent of other filterable it must be tried last
		else if( dynamic_cast< exFMessage const * >(&m) ) {// PRQA S 3081
            messageType = "Info" ; 
        }
        else {
            ;
        }
	}
	catch (exception &e)
	{
		messageType = "Error (ADGSLogFmtFactory level exception)" ;
	}

	string out ;
    return format(out, messageType, m.message()) ;
	

}

string ADGSLogFmtFactory::text(const exception &x, bool stack) const // PRQA S 4020
{
    const exException *ec = dynamic_cast<const exCriticalException *>(&x) ; // PRQA S 3081
    string messageType = "Error" ;
    if (ec) {
        messageType = "Critical" ;
    }
    else {
    }
    
    string out ;
    return format(out, messageType, exMsgFmt::text(x, stack)) ;
}



_ACS_END_NAMESPACE
