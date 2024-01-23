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

  $Prod: LTA Rest Interface $


*/

#include <GenericAuthHandler.h>

_ACS_BEGIN_NAMESPACE(acs)

GenericAuthHandler::GenericAuthHandler(AuthType type) :
  _type(type)
{
}

// LCOV_EXCL_START
// Defensive coding: not instantiable
GenericAuthHandler::~GenericAuthHandler() ACS_NOEXCEPT {}
// LCOV_EXCL_STOP

_ACS_END_NAMESPACE
