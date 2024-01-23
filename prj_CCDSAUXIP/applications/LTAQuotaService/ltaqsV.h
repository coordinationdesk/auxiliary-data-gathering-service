/*

    Copyright 1995-2022, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

    $Prod: LTAQuotaService $

*/

#ifndef _ltaqsV_H_
#define _ltaqsV_H_

namespace acs
{
// support class for versioning

class ltaqsV
{
public:
  ltaqsV() noexcept = default;
  ~ltaqsV() noexcept = default;
  static const char* tag() noexcept
  {
    return _tag;
  }
  static const char* name() noexcept
  {
    return _name;
  }

private:
  static const char* _tag;
  static const char* _name;
};

}  // namespace acs

#endif  // _ltaqsV_H_
