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

#include <exMacros.h>
#include <exException.h>
#include <Filterables.h>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>
#include <dbConnection.h>
#include <BaseMetricsHandler.h>
#include <MonthlyMetricsHandler.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MonthlyMetricsHandler);

MonthlyMetricsHandler::MonthlyMetricsHandler() :
	BaseMetricsHandler()
{ }

void MonthlyMetricsHandler::computeAndStore(dbConnection& conn, const DateTime& ref) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	std::map<std::string, float> monthlyMin;
	std::map<std::string, float> monthlyMax;
	std::map<std::string, float> monthlyAvg;
	
	DateTime start = ref - 30.0;
	
	loadMetrics("MIN", "\%Daily.min\%", start, ref, monthlyMin, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << monthlyMin.size() << " MIN metrics.");
	
	loadMetrics("MAX", "\%Daily.max\%", start, ref, monthlyMax, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << monthlyMax.size() << " MAX metrics.");

	loadMetrics("AVG", "\%Daily.avg\%", start, ref, monthlyAvg, conn);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << monthlyAvg.size() << " AVG metrics.");

	
	// Store monthly metrics on database
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing monthly metrics on database");
	if (!monthlyMin.empty()) {
		storeMetrics(monthlyMin, ref, conn);
		ACS_LOG_INFO("Stored " << std::to_string(monthlyMin.size()) << " MIN monthly metrics into database");
	} else {
		ACS_LOG_INFO("No MIN monthly metrics to be stored on database");
	}
	
	if (!monthlyMax.empty()) {
		storeMetrics(monthlyMax, ref, conn);
		ACS_LOG_INFO("Stored " << std::to_string(monthlyMax.size()) << " MAX monthly metrics into database");
	} else {
		ACS_LOG_INFO("No MAX monthly metrics to be stored on database");
	}
	
	if (!monthlyAvg.empty()) {
		storeMetrics(monthlyAvg, ref, conn);
		ACS_LOG_INFO("Stored " << std::to_string(monthlyAvg.size()) << " AVG monthly metrics into database");
	} else {
		ACS_LOG_INFO("No AVG monthly metrics to be stored on database");
	}
}

void MonthlyMetricsHandler::storeMetrics(const std::map<std::string, float>& mets, const DateTime& ref, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", MonthlyMetricsHandlerException("Unsupported dialect: " + dialectName));

	std::stringstream ssbase;
	ssbase << "INSERT INTO t_srv_metrics(name, tstamp, metrictype, gauge) VALUES ";
	
	std::stringstream ss; 
	size_t blkcount = 0;
	size_t idx = 0;
	std::map<std::string, float>::const_iterator it = mets.begin();
	while (it != mets.end()) {
		std::string newname = StringUtils::transform(it->first, ".Daily.", ".Monthly.");
		
		ss << "(\'" << newname << "\',";
		ss << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ss << "\'Gauge\',";
		ss << it->second << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ss.str());

		++blkcount;
		
		if (blkcount >= maxInsertBlock_ || idx == mets.size() - 1) {
			SQLString sqlIns(dbKey, SQLString::COMMAND);
			sqlIns.forceSQLString(ssbase.str() + ss.str());
			
			dbQuery query(conn);
			query.sqlString(sqlIns, "InsertMonthlyMetrics");
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << query.sqlString());
			
			if (!query.execForStatus()) {
				ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + sqlIns.getSQL() + "\"Error was: " + std::string(query.errorString())));
			}
			
			ss.str("");
			blkcount = 0;
		}
		else {
			ss << ",";
		}
		
		std::advance(it, 1);
		++idx;
	}
}

void MonthlyMetricsHandler::loadMetrics(	const std::string& groupByFunc, 
										const std::string& sqlLikeNamePatt, 
										const DateTime& start, 
										const DateTime& stop, 
										std::map<std::string, float>& metric, 
										dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	SQLString sql(dbKey, SQLString::SELECT);
	sql.addTableList("t_srv_metrics");
	sql.addSelectList("name");
	
	{	// Reduction function
		SQLString sqlfunc(dbKey, SQLString::FUNCTION);
		sqlfunc.setFunctionName(groupByFunc);
		sqlfunc.addFunctionParameter("gauge");
	
		SQLDialect::SQLCast sqlCast(SQLDialect::SQL_FLOAT) ;
		sql.addSelectList(sqlfunc.getSQL(true), sqlCast, "g");
	}
	
	// WHERE condition
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_srv_metrics", "name", dbParam::LIKE, dbConv(dbKey, sqlLikeNamePatt)));
		pp.addParameter(dbParam(dbKey, "t_srv_metrics", "tstamp", dbParam::BETWEEN, 
				db::toSQL(dbKey, start, dbConv::YearToFraction), db::toSQL(dbKey, stop, dbConv::YearToFraction)));
		sql.addANDWhereParameter(pp);
	}

	// Group by name
	sql.addGroupByList("name");
	
	dbQuery query(conn);
	query.sqlString(sql, "Load" + groupByFunc + "MetricsInInterval");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query: " << query.sqlString());
	
	dbSet set;
	query.execForSet(set);
	
	dbRow row(dbKey);
	while (set.getNextRow(row)) {
		std::string name;
		float gauge;
		
		row.column(0, name);
		row.column(1, gauge);
		
		metric[name] = gauge;
	}
}


_ACS_END_NAMESPACE
