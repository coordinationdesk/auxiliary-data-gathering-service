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

#ifndef _ODataNotificationServicePayload_h_
#define _ODataNotificationServicePayload_h_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <string>
#include <NotificationServiceRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful notification sent by LTA */
class ODataNotificationServicePayload : public NotificationServiceRequestPayload { 
public:
	
	exDECLARE_EXCEPTION(ODataNotificationServicePayloadException, exException);

	/** Ctors/Dtors */
	explicit ODataNotificationServicePayload(size_t id);
	explicit ODataNotificationServicePayload(const std::string& jsstr);
	explicit ODataNotificationServicePayload(const nlohmann::json& js);
	ODataNotificationServicePayload(const ODataNotificationServicePayload&);
	virtual ~ODataNotificationServicePayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	ODataNotificationServicePayload& operator=(const ODataNotificationServicePayload&);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataNotificationServicePayload);
};

_ACS_END_NAMESPACE
#endif /* _ODataNotificationServicePayload_h_ */
