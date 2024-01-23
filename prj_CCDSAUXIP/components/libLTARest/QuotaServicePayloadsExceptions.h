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

#ifndef _QuotaServicePayloadsExceptions_h_
#define _QuotaServicePayloadsExceptions_h_
#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, QuotaServicePayloadsExceptions)

exDECLARE_EXCEPTION(RequestPayloadException, exException);

_ACS_END_NESTED_NAMESPACE
#endif /* _QuotaServicePayloadsExceptions_h_ */