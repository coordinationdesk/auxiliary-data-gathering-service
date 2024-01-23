// PRQA S 1050 EOF
/*

	Copyright 2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Metrics Library $


*/

#include <MetricsDbFunctions.h>
#include <SQLDialectFactory.h>
#include <SQLString.h>
#include <dbQuery.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MetricsDbFunctions);

std::string MetricsDbFunctions::getNowSql(const std::string& dbKey) {
	std::string nowString;
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	if( dialectName == "Postgres8") {
		nowString = "now()";
	}
	else if (dialectName == "Oracle11") {
		nowString = "LOCALTIMESTAMP";
	}
	else {
		ACS_THROW(MetricsFunctionsException("Database dialect not supported: " + dialectName));
	}
	
	return nowString;
}


bool MetricsDbFunctions::cleanupAgedMetrics(unsigned long long secs, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	SQLString sqlDel(dbKey, SQLString::DELETE);
	sqlDel.addTableList("t_srv_metrics");
	{
		SQLString agingSql(dbKey, SQLString::COMMAND);
		agingSql.forceSQLString(SQLString::intervalToSeconds(dbKey, "tstamp",  dbConv(dbKey, getNowSql(dbKey))));
		
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, dbParam::MAJOR_OF, agingSql, db::toSQL(dbKey, secs)));
		sqlDel.addANDWhereParameter(pp);
	}
	
	dbQuery query(conn);
	query.sqlString(sqlDel, "CleanupAgedMetrics");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query: " << query.sqlString());
	
	return query.execForStatus();
	
}

_ACS_END_NAMESPACE
