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


#ifndef _IPFLogConfiguration_H_
#define _IPFLogConfiguration_H_ 

#include <Singleton.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


/*! \class IPFLogConfiguration: this is a utility class used to handle the Log Levels.
	
*/
class IPFLogConfiguration : public Singleton <IPFLogConfiguration> // PRQA S 2109, 2153
{

    // Friendship is needed by the Singleton pattern
    friend IPFLogConfiguration* Singleton<IPFLogConfiguration>::instance(); // PRQA S 2107

public:

    // Enum are sorted and so this list defines also the prioirty sorting of message types
	enum LogLevel { Debug = 0, Info,  Progress, Warning , Error };

	virtual ~IPFLogConfiguration() throw() ;

    /** set the  log level*/
    void setLogLevel(const LogLevel&) throw();
    /** return the log level (default is Debug) */
    const LogLevel& getLogLevel() const throw();


private:
	IPFLogConfiguration(); // not implemented
	IPFLogConfiguration(const IPFLogConfiguration &); // not implemented
	IPFLogConfiguration &operator=(const IPFLogConfiguration &); // not implemented
private:
    //attributes
    LogLevel _logLevel; //the log level 

};


_ACS_END_NAMESPACE


#endif // _IPFLogConfiguration_H_
