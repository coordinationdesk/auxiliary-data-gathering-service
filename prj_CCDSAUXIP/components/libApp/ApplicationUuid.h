// PRQA S 1050 EOF
/*
	Copyright 2020, Exprivia SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it


	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. libApp
*/

#ifndef _ApplicationUuid_H_
#define _ApplicationUuid_H_

#include <acs_c++config.hpp>
#include <File.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * ApplicationUuid class.
 * The class is a commodity to allow only an application instance to have the same Uuid
 * on the same host.
 */

class ApplicationUuid // PRQA S 2109
{
public:
	explicit ApplicationUuid(const std::string& lockName) ACS_NOEXCEPT ;
	virtual ~ApplicationUuid() ACS_NOEXCEPT ;
	
	std::string getUuid();
	
private:
	//Not implemented to prevent their use
	ApplicationUuid(ApplicationUuid const &) = delete;
	ApplicationUuid & operator=(ApplicationUuid const &) = delete;

	/**
        	Create and Lock the file
	*/
	const std::string &lockAndCreate(bool exception = true) ;
	void writeUuid();
	std::string readUuid();
	void generateNewUuid();


private:
	
	std::string _baseLockName ; 	// base lock name (without application instance number)
	std::string _actualLockName ; 	// application uuid name (to be locked)
	File::Lock _lock ;
	std::string _uuid;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ApplicationUuid) ;
};

_ACS_END_NAMESPACE

#endif /* _ApplicationUuid_H_ */
