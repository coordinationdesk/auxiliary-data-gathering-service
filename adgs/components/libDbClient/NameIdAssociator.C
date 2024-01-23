// PRQA S 1050 EOF
/*
	Copyright 1995-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

*/

#include <NameIdAssociator.h>


_ACS_BEGIN_NAMESPACE(acs)

NameIdAssociatorConfiguration::NameIdAssociatorConfiguration () :
	Singleton < NameIdAssociatorConfiguration > (),
	_mutex(),
	_enableDiskMaps(true)
{}

NameIdAssociatorConfiguration::~NameIdAssociatorConfiguration() ACS_NOEXCEPT
{
}

void NameIdAssociatorConfiguration::setEnableDiskMaps (bool enable)
{
	Mutex::Lock lock (_mutex);
	_enableDiskMaps=enable;
}

bool NameIdAssociatorConfiguration::getEnableDiskMaps () const
{
	Mutex::Lock lock (_mutex);
	return _enableDiskMaps;
}

_ACS_END_NAMESPACE
