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


#ifndef _ADGSLogFmt_H_
#define _ADGSLogFmt_H_

#include <acs_c++config.hpp>
#include <exMsgFmtFactory.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class ADGSLogFmtFactory : public exMsgFmtFactory // PRQA S 2109, 2153
{
public:
	ADGSLogFmtFactory() ;
	virtual ~ADGSLogFmtFactory() throw() ;
	/** convenience ctor for inheritance without need to call 
	  * the sequence emitPid(bool) ... emitTime(bool) ...
	  */
	ADGSLogFmtFactory(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) ;

	//ADGSLogFmtFactory(const std::string &sep) ;

	/**
	  * Re-implements standard formatter adding message type in message
	  */
	virtual std::string text(const exFilterable &) const ;
	virtual std::string text(const std::exception &, bool stack) const ;

private:
    std::string &format(std::string &out, const std::string &messageType, const std::string &message) const ;
    

private:
	// declared but not implemented
	ADGSLogFmtFactory(const ADGSLogFmtFactory &) ;
	ADGSLogFmtFactory &operator=(const ADGSLogFmtFactory &) ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ADGSLogFmtFactory) ;

};

_ACS_END_NAMESPACE

#endif //_ADGSLogFmt_H_

