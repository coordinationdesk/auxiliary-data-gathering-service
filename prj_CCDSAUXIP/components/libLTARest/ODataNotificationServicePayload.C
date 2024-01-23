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

#include <ODataNotificationServicePayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataNotificationServicePayload);

ODataNotificationServicePayload::ODataNotificationServicePayload(size_t id) : NotificationServiceRequestPayload(id, true)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}
ODataNotificationServicePayload::ODataNotificationServicePayload(const std::string& jsstr) : NotificationServiceRequestPayload(jsstr, true)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}
ODataNotificationServicePayload::ODataNotificationServicePayload(const nlohmann::json& js) : NotificationServiceRequestPayload(js, true)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}
ODataNotificationServicePayload::ODataNotificationServicePayload(const ODataNotificationServicePayload& data) : NotificationServiceRequestPayload(data)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}
ODataNotificationServicePayload::~ODataNotificationServicePayload() ACS_NOEXCEPT { }

ODataNotificationServicePayload& ODataNotificationServicePayload::operator=(const ODataNotificationServicePayload& data) 
{
	if (this != &data)
	{
		NotificationServiceRequestPayload::operator=(data);
	}
  	return *this;
}
_ACS_END_NAMESPACE
