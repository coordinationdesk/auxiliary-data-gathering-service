// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. MQTT Statistics Library
*/

#include <auto_version_file>
#include <mqttstatV.h>

namespace acs {

const char* mqttstatV::_name = auto_version_file::name;
const char* mqttstatV::_tag = auto_version_file::tag;


mqttstatV::mqttstatV() ACS_NOEXCEPT {}
mqttstatV::~mqttstatV() ACS_NOEXCEPT {}


const char*  mqttstatV::tag() { return _tag ; }
const char*  mqttstatV::name() { return _name ; }

} // namespace acs
