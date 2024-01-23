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
	Revision 5.1  2014/05/20 12:16:13  marfav
	Addedd support for IPF Standalone app
	

*/


#include <IPFLogLevelFilter.h>
#include <Filterables.h>

#include <IPFLogConfiguration.h>


using namespace acs ;
using namespace std ;



//
//
// IPFLogLevelFilter
//
//


IPFLogLevelFilter::IPFLogLevelFilter(exostream *s) : exFilter(s)
{
}

IPFLogLevelFilter::~IPFLogLevelFilter() throw()
{
}


IPFLogLevelFilter::IPFLogLevelFilter(const IPFLogLevelFilter& f) : exFilter(f)
{
}

IPFLogLevelFilter& IPFLogLevelFilter::operator= (const IPFLogLevelFilter& f)
{
	if (this != &f)
	{
		// base class copy
		exFilter::operator= (f);
	}

	return *this;
}


bool IPFLogLevelFilter::filterInsertion(exFilterable &fo) 
{

    IPFLogConfiguration::LogLevel logLevel = IPFLogConfiguration::instance()->getLogLevel();
    IPFLogConfiguration::LogLevel msgLevel = IPFLogConfiguration::Debug;

    if (0 != dynamic_cast<exFDebug*> (&fo)) // PRQA S 3081 L_cast
    {
        msgLevel = IPFLogConfiguration::Debug;
    }
    else if (0 != dynamic_cast<exFWarning*> (&fo))
    {
        msgLevel = IPFLogConfiguration::Warning;
    }
    else if (0 != dynamic_cast<exFError*> (&fo))
    {
        msgLevel = IPFLogConfiguration::Error;
    }
    else if (0 != dynamic_cast<exFProgress*> (&fo))
    {
        msgLevel = IPFLogConfiguration::Progress;
    }
    else
    {
        msgLevel = IPFLogConfiguration::Info;
    }
    // PRQA L:L_cast
  	return (msgLevel >= logLevel);
}

