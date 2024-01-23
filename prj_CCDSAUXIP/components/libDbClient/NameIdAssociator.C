/*
  Copyright 1995-2022, Exprivia SpA - DFDA-AS
  Via Della Bufalotta, 378 - 00139 Roma - Italy
  http://www.exprivia.com

  All Rights Reserved.

  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
  the contents of this file may not be disclosed to third parties, copied or
  duplicated in any form, in whole or in part, without the prior written
  permission of Exprivia SpA

  $Prod: A.C.S. DataBase Client Library $

*/

#include <NameIdAssociator.h>

_ACS_BEGIN_NAMESPACE(acs)

void NameIdAssociatorConfiguration::setEnableDiskMaps(bool enable)
{
  Mutex::Lock lock(_mutex);
  _enableDiskMaps = enable;
}

bool NameIdAssociatorConfiguration::getEnableDiskMaps() const
{
  Mutex::Lock lock(_mutex);
  return _enableDiskMaps;
}

_ACS_END_NAMESPACE
