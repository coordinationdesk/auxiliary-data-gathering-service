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

#ifndef _RetrieveServiceResponseErrorCodeEnum_h_
#define _RetrieveServiceResponseErrorCodeEnum_h_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

enum RetrieveServiceResponseErrorCode
{
  RETRIEVESERVICE_PAYLOAD_PARSE_ERROR = 1,
  RETRIEVESERVICE_INTERNAL_ERROR = 2,
  RETRIEVESERVICE_PRODUCT_NOT_FOUND = 4,
  RETRIEVESERVICE_LTA_REQUEST_ERROR = 8,
  RETRIEVESERVICE_DB_ERROR = 16,
  RETRIEVESERVICE_LTA_SELECTION_ERROR = 32
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceResponseErrorCodeEnum_h_ */
