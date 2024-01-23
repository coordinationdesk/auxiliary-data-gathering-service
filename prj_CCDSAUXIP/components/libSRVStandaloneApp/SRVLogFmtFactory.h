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

	$Prod: LTA Standalone App $


*/


#ifndef _SRVLogFmtFactory_H_
#define _SRVLogFmtFactory_H_

#include <acs_c++config.hpp>
#include <exMsgFmtFactory.h>


_ACS_BEGIN_NAMESPACE(acs)

class SRVLogFmtFactory : public exMsgFmtFactory // PRQA S 2153
{
public:
	SRVLogFmtFactory() ;
	virtual ~SRVLogFmtFactory() throw() ;
	/** convenience ctor for inheritance without need to call 
	  * the sequence emitPid(bool) ... emitTime(bool) ...
	  */
	SRVLogFmtFactory(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) ;

	/**
	  * Un-hide the exMsgFmtFactory methods
	  */
	using exMsgFmtFactory::text; 

	/**
	  * Re-implements standard formatter adding message type in message
	  */
	virtual std::string text(const exFilterable &) const ;


private:
	// declared but not implemented
	SRVLogFmtFactory(const SRVLogFmtFactory &) ;
	SRVLogFmtFactory &operator=(const SRVLogFmtFactory &) ;

};


_ACS_END_NAMESPACE

#endif //_SRVLogFmtFactory_H_

