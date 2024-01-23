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


#ifndef _osfiIPFLogFmtFactory_H_
#define _osfiIPFLogFmtFactory_H_

#include <exMsgFmtFactory.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

/**  
 * \class osfiIPFLogFmtFactory
 * \brief Factory for formatting IPF logs in compliance with openSF
 */
class osfiIPFLogFmtFactory : public exMsgFmtFactory // PRQA S 2109, 2153
{
public:
	/** 
	 * \brief Default constructor 
	 */
    osfiIPFLogFmtFactory() ;
    
    /** 
	 * \brief Default Destructor 
	 */
	virtual ~osfiIPFLogFmtFactory() throw() ;
	
    /** 
	 * \brief Constructor
     * \param pid boolean indicating if emitting pid or not
     * \param time boolean indicating if emitting time or not
     * \param start string added at the beginning of each message
     * \param end string added at the end of each message
     * \param sep string indicating the separator
	 */
    osfiIPFLogFmtFactory(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) ;
	
	/**
	  * Un-hide the exMsgFmtFactory methods
	  */
	using exMsgFmtFactory::text; 

    /** 
	 * \brief Set processor name 
     * \param s string reporting the processor name
	 */
    void setProcName(const std::string& s) throw() {_procName = s;}
    
    /** 
	 * \brief Set processor version 
     * \param s string reporting the processor version
	 */
    void setProcVersion(const std::string& s) throw() {_procVersion = s;}
    
    /** 
	 * \brief Set task name
     * \param s string reporting the task name 
	 */
    void setTaskName(const std::string& s) throw() {_taskName = s;}
    
    /** 
	 * \brief Set the end line character 
     * \param c char reporting the end line character
	 */
    void setEndLine(const char& c) throw() {_endLine = c;}
    
    /** 
	 * \brief Enable colored messages (default: true) 
     * \param on boolean equals to true if setting colored messages
	 */
    void setColorOn(const bool& on) throw() {_colorOn = on;}
    
    /** 
	 * \brief Set the flag indicating if enabling or nor openSF format logs (default: true) 
     * \param on boolean equals to true if enabling messages in openSF format
	 */
    void enablingOsfiFmt(const bool& on) throw() {_osfiFmtOn = on;}
    
	/** 
	 * \brief Set the flag indicating if running in openSF framework (default: false) 
     * \param on boolean equals to true if disabling some strings in the messages
	 */
    void runByOpenSF(const bool& on) throw() {_runByOpenSF = on; emitPid(on? false:true); emitTime(on? false:true); }
	
	/**
	  * \brief Re-implements standard formatter adding message type in message
      * \param exFilterable includes the message to be formatted
	  */
	virtual std::string text(const exFilterable &) const ;
	
    /** 
	 * \brief Returns the configured processor name 
	 */
	const std::string& procName() const throw() { return _procName; }
    
    /** 
	 * \brief Returns the configured processor version 
	 */
	const std::string& procVersion() const throw() { return _procVersion; }
    
    /** 
	 * \brief Returns the configured task name 
	 */
	const std::string& taskName() const throw() { return _taskName; }
    
    /** 
	 * \brief Returns the configured end line character 
	 */
	const char& endLine() const throw() { return _endLine; }
    
    /** 
	 * \brief Returns a boolean indicating if colored messages are enabled or not 
	 */
	const bool& colorOn() const throw() { return _colorOn; }
    
    /** 
	 * \brief Returns a boolean indicating if openSF format is enabled or not 
	 */
    const bool& osfiFmtEnabled() const throw() { return _osfiFmtOn; }
    
    /** 
	 * \brief Returns the openSF version (empty if openSF is not installed) 
	 */
    const std::string& osfiVersion() const throw() { return _osfiVersion; }
    
private:
	// declared but not implemented
	osfiIPFLogFmtFactory(const osfiIPFLogFmtFactory &) ;
	osfiIPFLogFmtFactory &operator=(const osfiIPFLogFmtFactory &) ;
	
	std::string		_procName ;
	std::string		_procVersion ;
	std::string		_taskName ;
	char			_endLine;
    bool            _colorOn ;
    bool            _osfiFmtOn ;
	bool			_runByOpenSF ;
    std::string		_osfiVersion ;

private:
    static const std::string osfiVersionFileName ;
    static const std::string osfiMsgDelimiter ;
    
    /** 
	 * \brief Recover the openSF version (if installed) 
	 */
    void setOsfiVersion() ;
    
    /** 
	 * \brief Format a message in openSF error format 
	 */
    std::string error_osfiFmt(const std::string& message) const ;
    
    /** 
	 * \brief Format a message in openSF warning format 
	 */
    std::string warning_osfiFmt(const std::string& message) const ;
    
    /** 
	 * \brief Format a message in openSF information format 
	 */
    std::string info_osfiFmt(const std::string& message) const ;
    
    /** 
	 * \brief Format a message in openSF debug format 
	 */
    std::string debug_osfiFmt(const std::string& message) const ;
    
    /** 
	 * \brief Format a message in openSF progress status format 
	 */
    std::string progress_osfiFmt(const std::string& message) const ;
    
    /** 
	 * \brief Format a message in openSF format 
	 */
    std::string toOsfiFmt(const std::string& messageType, const std::string& prefix, const std::string& message) const ;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(osfiIPFLogFmtFactory) ;

};

_ACS_END_NAMESPACE

#endif //_osfiIPFLogFmtFactory_H_

