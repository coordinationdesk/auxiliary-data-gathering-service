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


#ifndef _osfiIPFStandaloneApp_H_
#define _osfiIPFStandaloneApp_H_

#include <StandaloneApp.h>
#include <Filterables.h>
#include <osfiIPFLogFmtFactory.h>
#include <IPFLogLevelFilter.h>
#include <IPFLogConfiguration.h>

_ACS_BEGIN_NAMESPACE(acs)

/**  
 * \class osfiIPFStandaloneApp
 * \brief StandAlone App instantiating the FLEX specifc log formatter
 */
class osfiIPFStandaloneApp : public StandaloneApp // PRQA S 2109
{

public:
    /**  
     * \class osfiIPFLogStream
     * \brief Class redirecting all IPF logs to stdout stream.
     */
    class osfiIPFLogStream : public exBaseStream // PRQA S 2153
    {
    public:
    	osfiIPFLogStream() : exBaseStream("osfiIPFLogStream") {} 
		virtual ~osfiIPFLogStream() throw () { flush() ;} // PRQA S 2131, 4631

        virtual void notifyFilterable (exFilterable& f) // PRQA S 2131
		{
			if (dynamic_cast<exFError *>(&f)) // PRQA S 3081 L1
			{ 
				excout << dynamic_cast<exFError&> (f);
			}
			else if (dynamic_cast<exFProgress *>(&f)) 
			{ 
				excout << dynamic_cast<exFProgress&> (f);
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
    	osfiIPFLogStream(const osfiIPFLogStream &) ; // not implemented
    	osfiIPFLogStream& operator=(const osfiIPFLogStream &) ; // not implemented     
    };


	
public:
	/** 
	 * \brief Default constructor 
	 */
    osfiIPFStandaloneApp(std::string const & name, std::string const &subSystem, bool nomcf=true);
	
    /** 
	 * \brief Default Destructor 
	 */
    virtual ~osfiIPFStandaloneApp() throw() ;

protected:

	/**
	 * inherited from Application
	 */	
	virtual int start(int argc, char const * const * argv, char const * const * env);
    
    /** 
	 * \brief Set processor name 
     * \param s string reporting the processor name to be inserted in each message
	 */
    void setLogProcName(const std::string& s) throw(); 
    
    /** 
	 * \brief Set processor version 
     * \param s string reporting the processor version to be inserted in each message
	 */
    void setLogProcVersion(const std::string& s) throw();
    
    /** 
	 * \brief Set task name 
     * \param s string reporting the task name to be inserted in each message
	 */
    void setLogTaskName(const std::string& s) throw();
    
    /** 
	 * \brief Set the level of loggin 
     * \param LogLevel IPFLogConfiguration::LogLevel (Debug, Info,  Progress, Warning , Error)
	 */
    void setLogLevel(const IPFLogConfiguration::LogLevel&) throw();
    
    /** 
	 * \brief Set the end line char 
     * \param endLine char reporting the end line character
	 */
    void setLogEndLine(const char& endLine) throw();
    
    /** 
	 * \brief Set the flag indicating if emit or not the time in the logs 
     * \param b boolean set to true if enabling the emission of the time in the logs
	 */
    void setEmitTime(bool b) throw() ;
    
    /** 
	 * \brief Set the flag indicating if emit or not the pid in the logs 
     * \param b boolean set to true if enabling the emission of the pid in the logs
	 */
	void setEmitPid(bool b) throw() ;
	
    /** 
	 * \brief Set the flag indicating if enabling or nor openSF format logs (default: true) 
     * \param on boolean equals to true if enabling messages in openSF format
	 */
    void setColorOn(const bool& on) throw() {_theLogFmtFactory.setColorOn(on);}
    
    /** 
	 * \brief Set the flag indicating if enabling or nor openSF format logs (default: true) 
     * \param on boolean equals to true if enabling messages in openSF format
	 */
    void enablingOsfiFmt(const bool& on) throw() {_theLogFmtFactory.enablingOsfiFmt(on);}
    
	/** 
	 * \brief Set the flag indicating if running in openSF framework (default: false) 
     * \param on boolean equals to true if disabling some strings in the messages
	 */
    void runByOpenSF(const bool& on) throw() {_theLogFmtFactory.runByOpenSF(on); }
	
private:

	// declared but not implemented to prevent their use
	osfiIPFStandaloneApp();
	osfiIPFStandaloneApp(const osfiIPFStandaloneApp & );
	osfiIPFStandaloneApp &operator=(const osfiIPFStandaloneApp & );
	
	void attachLogHandler() ;
   	void detachLogHandler() ;
	
private:
    osfiIPFLogFmtFactory _theLogFmtFactory;
    osfiIPFLogStream _theLogStream;
    IPFLogLevelFilter _thelogFilter;
    exostream* _defaultOutLogger;

	// std::auto_ptr<coutIPFstream> _coutIPFstream ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(osfiIPFStandaloneApp) ;
};

_ACS_END_NAMESPACE

#endif //_osfiIPFStandaloneApp_H_
