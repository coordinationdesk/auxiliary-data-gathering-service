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

	$Prod: SRV Standalone App $


*/



#include <SRVLogFmtFactory.h>

#include <Filterables.h>
#include <DateTime.h>
#include <Application.h>



_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


SRVLogFmtFactory::SRVLogFmtFactory() :
	exMsgFmtFactory()
{
    exMsgFmtFactory::emitTime(true) ;
}


SRVLogFmtFactory::SRVLogFmtFactory(bool pid, bool time, const string &start, const string &end, const string &sep) :
	exMsgFmtFactory(pid,  time,   start,   end,  sep)
{}

SRVLogFmtFactory::~SRVLogFmtFactory() throw()
{}


string SRVLogFmtFactory::text(const exFilterable &m) const 
{
	Lock lock(exostream::globalMutex()) ;
	string out ;

	string messageType = "UnkLevel" ;

	try
	{	
		// Note since there is a hierarchy of filerable classes dynamc casts 
        // should be tried in a precise (hyerarchical) order
		// 
		if( dynamic_cast< exFDebug const * >(&m) )             { messageType = "Debug" ; } // PRQA S 3081 9
		else if( dynamic_cast< exFError const * >(&m) )        { messageType = "Error" ; }
		else if( dynamic_cast< exFWarning const * >(&m) )      { messageType = "Warning" ; }
		else if( dynamic_cast< exFOpEventAlert const * >(&m) ) { messageType = "Alert" ; } // Note son of exFEvent
		else if( dynamic_cast< exFEvent const * >(&m) )        { messageType = "Notice" ; }
		else if( dynamic_cast< exFOpAlert const * >(&m) )      { messageType = "Alert" ; }
		
		// Note since exFMessage is the parent of other filterable it must be tried last
		else if( dynamic_cast< exFMessage const * >(&m) )      { messageType = "Info" ; }
        else {
            messageType = "UnkLevel" ;
        }
	}
	catch (exception &e)
	{
		messageType = "Error (SRVLogFmtFactory level exception)" ;
	}

	
	out += startTag() ;

	if (emitTime()) {
        ostringstream os ;
		DateTime().asISO8601(os,3) << separator() ; // PRQA S 4400
 		out += os.str() ;
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
	
	
	string msg = m.message() ;
	
	// probably not the best way but does the job.
	size_t last = msg.length()-1 ; // PRQA S 3084
	if (msg[last] == '\n') {
		msg[last] = ( not separator().empty() ) ?  (separator())[0] : ' ' ; // PRQA S 3380
	}
	out += msg ;
	
	out += endTag() ;
	
	
	return out ;

}

_ACS_END_NAMESPACE

