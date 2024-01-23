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


*/


#include <IPFLogConfiguration.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


//
//
// IPFLogConfiguration
//
//
IPFLogConfiguration::IPFLogConfiguration() : 
	Singleton<IPFLogConfiguration>(), 
	_logLevel(Debug)
{
}

IPFLogConfiguration::~IPFLogConfiguration() throw() 
{
}


void IPFLogConfiguration::setLogLevel(const LogLevel& l) throw() // PRQA S 4121
{
    _logLevel = l;
}


const IPFLogConfiguration::LogLevel& IPFLogConfiguration::getLogLevel() const  throw()// PRQA S 4120
{
    return _logLevel;
}

_ACS_END_NAMESPACE
