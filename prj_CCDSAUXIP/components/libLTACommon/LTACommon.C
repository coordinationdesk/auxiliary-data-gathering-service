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

	$Prod: LTA common functions $


*/

#include <LTACommon.h>

#include <RemoteSiteFactory.h>
#include <StringUtils.h>
#include <RegExp.h>
#include <algorithm>
#include <iterator>
#include <cstring>
#include <cmath>
#include <LTACommonV.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

namespace {
	LTACommonV version; // versioning
}

using namespace std;
using namespace curl;

RemoteSite compose_remote_site(const string& url, const string& bucket)
{
	RemoteSite site = RemoteSiteFactory::makeSite(url);
	site.setPath(StringUtils::pathNormalize(StringUtils::pathJoin(site.getPath(), bucket)));
	return site;
}

string uuidV1_bckt(const string &uuid, unsigned short quote)
{
	ACS_COND_THROW(quote == 0, exIllegalValueException("null quote")) ;
	ACS_COND_THROW(uuid.length() != 36, exIllegalValueException("invalid uuid: \"" + uuid + "\"")) ;

	unsigned short padlen = std::ceil(std::log10(quote));

	ostringstream os ;
	// actually reducing the low tstamp by 2^8 gives a reasonable likelyhood that each value would be hit
	// even if the precision of the timing is low
	os << std::setfill('0') << std::setw(padlen) << (strtoul(uuid.c_str(), nullptr, 16)>>8) % quote ;

	return os.str() ;
}

dbPersistent::IdType convertOrderStatus(dbConnection &conn, const OrderStatus status)
{
	switch (status)
	{
	case ORDERSTATUS_QUEUED:
		return LTAStatus::instance(conn.getKey())->id("SUBMITTED");
		break;
	case ORDERSTATUS_INPROGRESS:
		return LTAStatus::instance(conn.getKey())->id("RUNNING");
		break;
	case ORDERSTATUS_COMPLETED:
		return LTAStatus::instance(conn.getKey())->id("COMPLETED");
		break;
	case ORDERSTATUS_FAILED:
		return LTAStatus::instance(conn.getKey())->id("ERROR");
		break;
	case ORDERSTATUS_CANCELLED:
		return LTAStatus::instance(conn.getKey())->id("CANCELLED");
		break;

	default:
		return 0LL;
	}
}

OrderStatus convertOrderStatus(dbConnection &conn, const dbPersistent::IdType statusID)
{
	if (statusID == LTAStatus::instance(conn.getKey())->id("SUBMITTED"))
	{
		return ORDERSTATUS_QUEUED;
	}
	else if (statusID == LTAStatus::instance(conn.getKey())->id("RUNNING"))
	{
		return ORDERSTATUS_INPROGRESS;
	}
	else if (statusID == LTAStatus::instance(conn.getKey())->id("COMPLETED"))
	{
		return ORDERSTATUS_COMPLETED;
	}
	else if (statusID == LTAStatus::instance(conn.getKey())->id("ERROR"))
	{
		return ORDERSTATUS_FAILED;
	}
	else if (statusID == LTAStatus::instance(conn.getKey())->id("CANCELLED"))
	{
		return ORDERSTATUS_CANCELLED;
	}
	else
	{
		return ORDERSTATUS_INVALIDSTATUS;
	}
}

_ACS_END_NESTED_NAMESPACE
