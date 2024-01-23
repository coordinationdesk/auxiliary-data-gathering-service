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
	

*/


#ifndef _IPFLogFmtFactory_H_
#define _IPFLogFmtFactory_H_

#include <exMsgFmtFactory.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)


class IPFLogFmtFactory : public exMsgFmtFactory // PRQA S 2109, 2153
{
public:
	IPFLogFmtFactory() ;
	virtual ~IPFLogFmtFactory() throw() ;
	/** convenience ctor for inheritance without need to call 
	  * the sequence emitPid(bool) ... emitTime(bool) ...
	  */
    IPFLogFmtFactory(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) ;
	
	/**
	  * Un-hide the exMsgFmtFactory methods
	  */
	using exMsgFmtFactory::text; 

    void setProcName(const std::string& s) throw() {_procName = s;}
    void setProcVersion(const std::string& s) throw() {_procVersion = s;}
    void setTaskName(const std::string& s) throw() {_taskName = s;}
    void setEndLine(const char& c) throw() {_endLine = c;}
    
	/**
	  * Re-implements standard formatter adding message type in message
	  */
	virtual std::string text(const exFilterable &) const ;
	
	const std::string& procName() const throw() { return _procName; }
	const std::string& procVersion() const throw() { return _procVersion; }
	const std::string& taskName() const throw() { return _taskName; }
	const char& endLine() const throw() { return _endLine; }
	
private:
	// declared but not implemented
	IPFLogFmtFactory(const IPFLogFmtFactory &) ;
	IPFLogFmtFactory &operator=(const IPFLogFmtFactory &) ;
	
	std::string		_procName ;
	std::string		_procVersion ;
	std::string		_taskName ;
	char			_endLine;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(IPFLogFmtFactory) ;

};

_ACS_END_NAMESPACE

#endif //_IPFLogFmtFactory_H_

