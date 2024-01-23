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
	Revision 5.3  2015/07/15 06:51:08  ricfer
	setEmitTime and setEmitPid added.
	
	Revision 5.2  2014/07/15 11:44:40  nicvac
	Endline char handling.
	
	Revision 5.1  2014/05/20 12:16:13  marfav
	Addedd support for IPF Standalone app
	

*/


#ifndef _IPFStandaloneApp_H_
#define _IPFStandaloneApp_H_

#include <StandaloneApp.h>
#include <Filterables.h>
#include <IPFLogFmtFactory.h>
#include <IPFLogLevelFilter.h>
#include <IPFLogConfiguration.h>

_ACS_BEGIN_NAMESPACE(acs)

// This class does not override the StandaloneApp behavior
// it just instantiates the Sentinel-1 specifc log formatter

class IPFStandaloneApp : public StandaloneApp // PRQA S 2109
{

public:
    class IPFLogStream : public exBaseStream // PRQA S 2153
    {
    public:
    	IPFLogStream() : exBaseStream("IPFLogStream") {} 
		virtual ~IPFLogStream() throw () { flush() ;} // PRQA S 2131, 4631

        virtual void notifyFilterable (exFilterable& f) // PRQA S 2131
		{
			if (dynamic_cast<exFError *>(&f)) // PRQA S 3081 L1
			{ 
				excerr << dynamic_cast<exFError&> (f);
			}
			else if (dynamic_cast<exFProgress *>(&f)) 
			{ 
				excerr << dynamic_cast<exFProgress&> (f);
			}
			else if (dynamic_cast<exFWarning *>(&f)) 
			{
				excout << dynamic_cast<exFWarning&> (f);	
			}
			else if (dynamic_cast<exFDebug *>(&f)) 
			{
				excout << dynamic_cast<exFDebug&> (f);	
			}
			else 
			{
				excout << dynamic_cast<exFMessage&> (f);	
			}
            // PRQA L:L1
		}             

	private:
    	IPFLogStream(const IPFLogStream &) ; // not implemented
    	IPFLogStream& operator=(const IPFLogStream &) ; // not implemented     
    };


	
public:
	IPFStandaloneApp(std::string const & name, std::string const &subSystem, bool nomcf=true);
	virtual ~IPFStandaloneApp() throw() ;

protected:

	/**
	 * inherited from Application
	 */	
	virtual int start(int argc, char const * const * argv, char const * const * env);
    
    void setLogProcName(const std::string& s) throw(); 
    void setLogProcVersion(const std::string& s) throw();
    void setLogTaskName(const std::string& s) throw();
    
    void setLogLevel(const IPFLogConfiguration::LogLevel&) throw();
    void setLogEndLine(const char& endLine) throw();
    
    void setEmitTime(bool b) throw() ;
	void setEmitPid(bool b) throw() ;
	
private:

	// declared but not implemented to prevent their use
	IPFStandaloneApp();
	IPFStandaloneApp(const IPFStandaloneApp & );
	IPFStandaloneApp &operator=(const IPFStandaloneApp & );
	
	void attachLogHandler() ;
   	void detachLogHandler() ;
	
private:
    IPFLogFmtFactory _theLogFmtFactory;
    IPFLogStream _theLogStream;
    IPFLogLevelFilter _thelogFilter;
    exostream* _defaultOutLogger;

	// std::auto_ptr<coutIPFstream> _coutIPFstream ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(IPFStandaloneApp) ;
};

_ACS_END_NAMESPACE

#endif //_IPFStandaloneApp_H_

