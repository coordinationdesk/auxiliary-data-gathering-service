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

#ifndef _LTACommon_H_
#define _LTACommon_H_

#include <acs_c++config.hpp>

#include <NameIdAssociator.h>
#include <dbPersistent.h>
#include <RemoteSite.h>
#include <Statistics.h>
#include <string>

_ACS_BEGIN_NESTED_NAMESPACE(acs, lta)

namespace
{
	// LTAStatus
	const char ltastatTable[] = "t_srv_statuses";
	const char ltastatId[] = "id";
	const char ltastatName[] = "status";

	// LTASrvOpTypes
	const char ltaSrvOpTypesTable[] = "t_srv_op_types";
	const char ltaSrvOpTypesId[] = "id";
	const char ltaSrvOpTypesName[] = "type";

	// LTADlAIPSelAlgos
	const char ltaDlAIPSelAlgosTable[] = "t_ltadl_aip_sel_algos";
	const char ltaDlAIPSelAlgosId[] = "id";
	const char ltaDlAIPSelAlgosName[] = "name";

} // namespace

typedef NameIdAssociatorSingleton<ltastatTable, ltastatId, ltastatName, dbPersistent::IdType> LTAStatus;

typedef NameIdAssociatorSingleton<ltaSrvOpTypesTable, ltaSrvOpTypesId, ltaSrvOpTypesName, dbPersistent::IdType> LTASrvOpTypes;

typedef NameIdAssociatorSingleton<ltaDlAIPSelAlgosTable, ltaDlAIPSelAlgosId, ltaDlAIPSelAlgosName, dbPersistent::IdType> LTADlAIPSelAlgos;

// Global functions

inline acs::math::StatNode &stat() { return (*acs::math::Statistics::instance())()["LTA"]; } // PRQA S 2134, 2502

/*- returns a remote site consisting of the url path joined with the input
    bucket string.
*/
curl::RemoteSite compose_remote_site(const std::string &url, const std::string &bucket);

/*- returns a string to be used as bucket in generating AIP paths 
    it get uuid v1 in the form "b694b436-cf05-11e9-bb65-2a2ae2dbcce4" and uses only the very first 8 chars - tstamp low
    to compute the remainder of division by quote
*/
std::string uuidV1_bckt(const std::string &uuid, unsigned short quote = 100);

enum OrderStatus
{
	ORDERSTATUS_QUEUED,
	ORDERSTATUS_INPROGRESS,
	ORDERSTATUS_COMPLETED,
	ORDERSTATUS_FAILED,
	ORDERSTATUS_CANCELLED,
	ORDERSTATUS_INVALIDSTATUS
};

const std::map<OrderStatus, std::string> OrderStatusToString = {
	{ORDERSTATUS_QUEUED, "queued"},
	{ORDERSTATUS_INPROGRESS, "in_progress"},
	{ORDERSTATUS_COMPLETED, "completed"},
	{ORDERSTATUS_FAILED, "failed"},
	{ORDERSTATUS_CANCELLED, "cancelled"},
};
const std::map<std::string, OrderStatus> StringToOrderStatus = {
	{"queued", ORDERSTATUS_QUEUED},
	{"in_progress", ORDERSTATUS_INPROGRESS},
	{"completed", ORDERSTATUS_COMPLETED},
	{"failed", ORDERSTATUS_FAILED},
	{"cancelled", ORDERSTATUS_CANCELLED},
};

/*- convert the LTA Remote status into the id of the corresponding status in t_srv_statuses table
*/
dbPersistent::IdType convertOrderStatus(dbConnection &conn, const OrderStatus status);

/*- convert the the id of the status in t_srv_statuses table into the corresponding LTA Remote  status
*/
OrderStatus convertOrderStatus(dbConnection &conn, const dbPersistent::IdType status);

_ACS_END_NESTED_NAMESPACE

#endif /* _LTACommon_H_ */
