// PRQA S 1050 EOF
/*

    Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

    libResources  version file

*/ 

#include <auto_version_file>
#include <rsV.h>

namespace acs {

  const char* rsV::_name = auto_version_file::name;
  const char* rsV::_tag = auto_version_file::tag;

  const char* rsV::tag() noexcept { return _tag ; }
  const char* rsV::name() noexcept { return _name ; }

}

