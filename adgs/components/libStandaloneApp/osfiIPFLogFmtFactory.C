// PRQA S 1050 EOF
/*

	Copyright 2019-2029, Exprivia , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, Inc.

	$Prod: Exprivia FLEX L1BPP $

	$Id$

	$Author$

*/


#include <osfiIPFLogFmtFactory.h>

#include <Filterables.h>
#include <DateTime.h>
#include <RegExp.h>
#include <Application.h>
#include <osfi_vt100.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(osfiIPFLogFmtFactory) 

enum Keys {all, hostN, procN, procV, task};

const string osfiIPFLogFmtFactory::osfiVersionFileName("/Version") ;
const string osfiIPFLogFmtFactory::osfiMsgDelimiter("|") ;

osfiIPFLogFmtFactory::osfiIPFLogFmtFactory() :
	exMsgFmtFactory(true, 	// emit PID
			true, 			// emit Time
			"",   			// start TAG
			"",				// end TAG
			" "				// separator
			),
	_procName(""),
	_procVersion(""),
	_taskName(""),
	_endLine(' '),
    _colorOn(true),
    _osfiFmtOn(true),
	_runByOpenSF(false),
    _osfiVersion("")
{
    osfiIPFLogFmtFactory::emitTime(true) ;
    setOsfiVersion() ;
}

osfiIPFLogFmtFactory::osfiIPFLogFmtFactory(bool pid, bool time, const string &start, const string &end, const string &sep) :
	exMsgFmtFactory(pid,  time,   start,   end,  sep),
	_procName(""),
	_procVersion(""),
	_taskName(""),
	_endLine(' '),
    _colorOn(true),
    _osfiFmtOn(true),
	_runByOpenSF(false),
    _osfiVersion("")
{
}

osfiIPFLogFmtFactory::~osfiIPFLogFmtFactory() throw()
{}


string osfiIPFLogFmtFactory::text(const exFilterable &m) const 
{
	Lock lock(exostream::globalMutex()) ;

	string out ;

	//add time information + separator
	if (emitTime()) {
        ostringstream os ;
		DateTime().asISO8601(os,6) << separator() ; // PRQA S 4400
 		out += os.str() ;
    }
	
	if(!_runByOpenSF) 
	{
		//add specific application information : HostName + ProcName + ProcVersion
		out += Application::hostname() + separator() ;
	}
	
	if (!procName().empty()) 
	{
		out +=  (procName() + separator()) ;
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
		messageType = "Error osfiIPFLogFmtFactory level exception)" ;
	}
	
	if (not messageType.empty() && !_runByOpenSF) {
		out += messageType + separator() ;
	}
	
	if(!_runByOpenSF)
	{
    	//Free text begins with the task name
    	out += taskName() ;
		if (!taskName().empty()) {
        	out += ':' + separator() ;
    	}
	}else
	{
		out += ':' + separator() ;
	}
	string msg = m.message() ;
	
	// probably not the best way but does the job.
	size_t last = msg.length()-1 ; // PRQA S 3084
	if (msg[last] == '\n') {
		msg[last] = (not separator().empty()) ?  separator()[0] : ' ' ; // PRQA S 3380
    }
    
    if(_osfiFmtOn)
    {
        out = toOsfiFmt(messageType, out, msg) ;
    }
    else
    {
        out += msg ;
    }
    
    //If configured, remove all '\n' from message.
	if ( _endLine != '\n' ) {
		std::replace( out.begin(), out.end(), '\n', _endLine);
	}
    
	return( out ) ;
}

void osfiIPFLogFmtFactory::setOsfiVersion()
{
	if(_osfiVersion.empty())
	{
		string line;
		char * osfiHome = getenv("OSFI_HOME");
		if (osfiHome)
		{
			string path = string(osfiHome) + osfiVersionFileName ;
			ifstream myfile (path.c_str());
			getline (myfile,line);
			myfile.close();
		}
		_osfiVersion = line;
	}
}

/**
 * Shows a formatted error message (mostly from OSFI source code).
 * @param message - text of the message
 */
string osfiIPFLogFmtFactory::error_osfiFmt(const string& message) const
{
	ostringstream os ;
    if(_colorOn)
    {
		os << set_colors( VT_RED , VT_DEFAULT ) << "Error    " << finalize << osfiMsgDelimiter << " " << message<< " "
		<< osfiMsgDelimiter << " " << _osfiVersion << endl;
	}
    else
	{
    	os <<  "Error    " <<  osfiMsgDelimiter << " " << message << " " << osfiMsgDelimiter << " " << _osfiVersion  << endl;
    }
    return(os.str()) ;
}

/**
 * Builds a formatted warning message (mostly from OSFI source code).
 * @param message - text of the message
 */
string osfiIPFLogFmtFactory::warning_osfiFmt(const string& message) const
{
	ostringstream os ;
    if(_colorOn)
    {
		os << set_colors( VT_YELLOW , VT_DEFAULT ) << "Warning  " << finalize << osfiMsgDelimiter << " " << message	<< " "
		<< osfiMsgDelimiter << " " << _osfiVersion << endl;
	}
    else
	{
    	os <<  "Warning  "  << osfiMsgDelimiter << " " << message << " " << osfiMsgDelimiter << " " << _osfiVersion  << endl;
    }
    return(os.str()) ;
}

/**
 * Builds a formatted information message (mostly from OSFI source code).
 * @param message - text of the message
 */
string osfiIPFLogFmtFactory::info_osfiFmt(const string& message) const
{
	ostringstream os ;
    if(_colorOn)
    {
		os << set_colors( VT_GREEN , VT_DEFAULT ) << "Info     " << finalize << osfiMsgDelimiter << " " << message << " "
		    << osfiMsgDelimiter << " " << _osfiVersion << endl;
	}
    else
	{
    	os <<  "Info     " <<  osfiMsgDelimiter << " " << message << " " << osfiMsgDelimiter << " " << _osfiVersion << endl;
    }
    return(os.str()) ;
}

/**
 * Builds a formatted debug message (mostly from OSFI source code).
 * @param message - text of the message
 */
string osfiIPFLogFmtFactory::debug_osfiFmt(const string& message) const
{
	ostringstream os ;
    os << "Debug    " << osfiMsgDelimiter << " " << message << " " << osfiMsgDelimiter << " " << _osfiVersion  << endl;
    return(os.str()) ;
}

/**
 * Shows a formatted progress message.
 * @param step - current step number
 * @param nSteps - maximum number of steps.
 */
string osfiIPFLogFmtFactory::progress_osfiFmt(const string& message) const
{
	vector<string> tokens;
    stringstream ss(message);
    string token ;
    while(std::getline(ss, token, ' ')) 
    {
        tokens.push_back(token);
    }
    
    int step = 0 ;
    int nSteps = 0 ;
    ostringstream os ;
    std::vector<string>::iterator it = std::find(tokens.begin(), tokens.end(), "STEP") ;
    if( (tokens.end() - it) > 2 )
    {
        ++it ;
        step = atoi((*it).c_str()) ;
        ++it ;
        nSteps = atoi((*it).c_str()) ;
    }
    
    // Put in OpenSF format
    const bool stepsOk = (nSteps>0 && step>0 && step<=nSteps) ;
    if(_colorOn)
    {
       os << set_colors( VT_CYAN , VT_DEFAULT ) << "Progress " << finalize << osfiMsgDelimiter << " " ; 
    }
    else
    {
        os <<  "Progress "  << osfiMsgDelimiter ;
    }
    if(stepsOk)
    {
        os << step << " of " << nSteps ;
    }
    else
    {
        os << message ;
    }
    os << " " << osfiMsgDelimiter << " "  << _osfiVersion  << " "<< endl;
    
    return(os.str()) ;
}

string osfiIPFLogFmtFactory::toOsfiFmt(
    const string& messageType, 
    const string& prefix,
    const string& message) const
{
	string outMsg(prefix+message) ;
    if(messageType == "[W]")
    {
        return(warning_osfiFmt(outMsg)) ;
    }
    else if(messageType == "[P]")
    {
        return(progress_osfiFmt(message)) ;
    }
    else if(messageType == "[E]")
    {
        return(error_osfiFmt(outMsg)) ;
    }
    else if(messageType == "[D]")
    {
        return(debug_osfiFmt(outMsg)) ;
    }
    else if(messageType == "[I]")
    {
        return(info_osfiFmt(outMsg)) ;
    }
    else
    {
        return(outMsg) ;
    }
}

_ACS_END_NAMESPACE

