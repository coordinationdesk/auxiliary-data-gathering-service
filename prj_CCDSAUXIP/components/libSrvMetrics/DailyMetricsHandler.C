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
#include <SQLDialectFactory.h>
#include <BaseMetricsHandler.h>
#include <DailyMetricsHandler.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DailyMetricsHandler);

DailyMetricsHandler::DailyMetricsHandler() :
	BaseMetricsHandler(),
	originToPublicationMetricsEnabled_(true),
	submissionToCompletionMetricsEnabled_(true),
	downloadMetricsEnabled_(true),
	archivedMetricsEnabled_(true),
	prodTypeArchivedMetricsEnabled_(true),
	retrievedMetricsEnabled_(true),
	includeMetricTypeInArchivedMetrics_(true)
{ }

void DailyMetricsHandler::computeAndStore(dbConnection& conn, const DateTime& ref) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	std::vector<OriginToPublicationMetrics> opmvect;
	std::vector<SubmissionToCompletionMetrics> scmvect;
	std::vector<ArchivedMetrics> arcvect;
	std::vector<RetrievedMetrics> retvect;
	std::vector<DownloadMetrics> dwnvect;
	
	// Load OriginToPublication metrics
	if (getOriginToPublicationMetricsEnabled()) {
		loadOriginToPublicationDailyMetrics(ref, opmvect, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed " << opmvect.size() << " OriginToPublication metrics");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "OriginToPublication metrics disabled.");
	}

	// Load SubmissionToCompletion metrics
	if (getSubmissionToCompletionMetricsEnabled()) {
		loadSubmissionToCompletionDailyMetrics(ref, scmvect, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed " << scmvect.size() << " SubmissionToCompletion metrics");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SubmissionToCompletion metrics disabled.");
	}
	
	// Load Archive metrics
	if (getArchivedMetricsEnabled()) {
		loadArchivedAndRetrievedMetrics(arcvect, retvect, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed " << arcvect.size() << " Archive metrics");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Archive metrics disabled.");
	}
	
	// Load Retrieved metrics
	if (getProdTypeArchivedMetricsEnabled()) {
		loadProdTypeArchivedMetrics(arcvect, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed " << retvect.size() << " prodtype Archived metrics");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Prodtype Archived metrics disabled.");
	}
	
	// Load Download metrics
	if (getDownloadMetricsEnabled()) {
		loadDownloadMetrics(dwnvect, conn);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed " << dwnvect.size() << " Download metrics");
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Download metrics disabled.");
	}
	
	// Store OriginToPublication metrics
	if (!opmvect.empty()) {
		storeOriginToPublicationDailyMetrics(ref, opmvect, conn);
		ACS_LOG_INFO("Stored " << std::to_string(opmvect.size()) << " OriginToPublication metrics into database");
	}
	else {
		ACS_LOG_INFO("No OriginToPublication metrics needs to be stored into database");
	}

	// Store SubmissionToCompletion metrics
	if (!scmvect.empty()) {
		storeSubmissionToCompletionDailyMetrics(ref, scmvect, conn);
		ACS_LOG_INFO("Stored " << std::to_string(scmvect.size()) << " SubmissionToCompletion metrics into database");
	}
	else {
		ACS_LOG_INFO("No SubmissionToCompletion metrics needs to be stored into database");
	}
	
	// Store Archived metrics
	if (!arcvect.empty()) {
		storeArchivedMetrics(ref, arcvect, conn);
		ACS_LOG_INFO("Stored " << std::to_string(arcvect.size()) << " Archived metrics into database");
	}
	else {
		ACS_LOG_INFO("No Archived metrics needs to be stored into database");
	}
	
	// Store Retrieved metrics
	if (!retvect.empty()) {
		storeRetrievedMetrics(ref, retvect, conn);
		ACS_LOG_INFO("Stored " << std::to_string(retvect.size()) << " Retrieved metrics into database");
	}
	else {
		ACS_LOG_INFO("No Retrieved metrics needs to be stored into database");
	}
	
	// Store Download metrics
	if (!dwnvect.empty()) {
		storeDownloadMetrics(ref, dwnvect, conn);
		ACS_LOG_INFO("Stored " << std::to_string(dwnvect.size()) << " Downloaded metrics into database");
	}
	else {
		ACS_LOG_INFO("No Downloaded metrics needs to be stored into database");
	}
}

/*
 * Used to compute OriginToPublications daily metrics using ref as reference time
 */
void DailyMetricsHandler::loadOriginToPublicationDailyMetrics(const DateTime& ref, std::vector<OriginToPublicationMetrics>& res, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	const std::string& dbKey = conn.getKey();

	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));

	std::stringstream ss;
	ss << "SELECT CASE substring(inv.name from 2 for 1)\n";
	ss << "    WHEN \'1\' then \'SENTINEL-1\'::varchar\n";
	ss << "    WHEN \'2\' then \'SENTINEL-2\'::varchar\n";
	ss << "    WHEN \'3\' then \'SENTINEL-3\'::varchar\n";
	ss << "    ELSE \'SENTINEL-UNKNOWN\'::varchar\n";
	ss << "END as platformshortname,\n";
	ss << "substring(inv.name from 3 for 1)::varchar as platformSerialIdentifier,\n";
	ss << "substring(tf.filetype from 4 for 20)::varchar as productType,\n";
	ss << "EXTRACT ('epoch' from MIN(inv.creationdate - inv.origindate)::interval second(0)) as minimum,\n";
	ss << "EXTRACT ('epoch' from MAX(inv.creationdate - inv.origindate)::interval second(0)) as maximum,\n";
	ss << "EXTRACT ('epoch' from AVG(inv.creationdate - inv.origindate)::interval second(0)) as average\n";
	ss << "FROM t_srv_inventory inv\n";
	ss << "INNER JOIN t_filetypes tf ON (tf.id_filetype = inv.filetype_id)\n";
	ss << "WHERE\n";
	ss << "    inv.creationdate BETWEEN " << db::toSQL(dbKey, ref - 1, dbConv::YearToFraction) << " AND " << db::toSQL(dbKey, ref, dbConv::YearToFraction) << "\n";
	ss << "    AND\n";
	ss << "    inv.mission IS NULL\n";
	ss << "GROUP BY platformshortname,platformSerialIdentifier,productType\n";
	ss << "ORDER BY platformshortname,platformSerialIdentifier,productType;";

	// force SQL query
	SQLString sql(dbKey, SQLString::COMMAND);
	sql.forceSQLString(ss.str());
	
	dbQuery query(conn);
	query.sqlString(sql, "LoadOriginToPublicationDailyMetricsHandler");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query:\n" << query.sqlString());
	
	dbSet set;
	query.execForSet(set);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << set.size() << " records");
	
	// Load the metrics
	dbRow row(dbKey);
	while (set.getNextRow(row)) {
		int i = 0;
		OriginToPublicationMetrics met; 
		
		std::string val;
		row.column(i++, met.platformShortName);
		row.column(i++, met.platformSerialId);
		row.column(i++, met.prodType);
		row.column(i++, met.min);
		row.column(i++, met.max);
		row.column(i++, met.avg);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded metric: " << met.platformShortName << "," << met.platformSerialId << "," << met.prodType << " -> <min,max,avg> = <" << met.min << "," << met.max << "," << met.avg << ">");
		
		res.push_back(met);
	}
}

/*
 * Used to compute SubmissionToCompletion daily metrics using ref as reference time
 */
void DailyMetricsHandler::loadSubmissionToCompletionDailyMetrics(const DateTime& ref, std::vector<SubmissionToCompletionMetrics>& res, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	std::stringstream ss;
	ss << "SELECT min(tab.submissiontocompletion) as minsubtocomp,\n";
	ss << "       max(tab.submissiontocompletion) as maxsubtocomp,\n";
	ss << "       avg(tab.submissiontocompletion) as avgsubtocomp\n";
	ss << "FROM (\n";
	ss << "    SELECT\n";
	ss << "        CASE (oq.completion_date > tru.creation_date)\n";
	ss << "            WHEN true THEN EXTRACT ('epoch' from (oq.completion_date - tru.creation_date)::interval second(0))\n";
	ss << "            ELSE EXTRACT ('epoch' from (oq.completion_date - tru.creation_date)::interval second(0))\n";
	ss << "        END as submissiontocompletion\n";
	ss << "    FROM t_srv_ordersqueue oq\n";
	ss << "    JOIN t_srv_request_x_user tru on (oq.id = tru.order_id)\n";
	ss << "WHERE oq.creation_date BETWEEN " << db::toSQL(dbKey, ref - 1, dbConv::YearToFraction) << " AND " << db::toSQL(dbKey, ref, dbConv::YearToFraction) << "\n";
	ss << ") as tab;";
	
	// force SQL query
	SQLString sql(dbKey, SQLString::COMMAND);
	sql.forceSQLString(ss.str());
	
	dbQuery query(conn);
	query.sqlString(sql, "LoadSubmissionToCompletionDailyMetricsHandler");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query: " << query.sqlString());
	
	dbSet set;
	query.execForSet(set);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << set.size() << " records");
	
	// Load the metrics
	dbRow row(dbKey);
	while (set.getNextRow(row)) {
		int i = 0;
		SubmissionToCompletionMetrics met; 
		
		bool isNull = false;
		row.column(i++, met.min, &isNull);
		if (isNull) {
			// min function returned null => SELECT returned no record set value to 0
			met.min = 0.0;
		}
		row.column(i++, met.max, &isNull);
		if (isNull) {
			// max function returned null => SELECT returned no record set value to 0
			met.max = 0.0;
		}
		row.column(i++, met.avg, &isNull);
		if (isNull) {
			// avg function returned null => SELECT returned no record, set value to 0
			met.avg = 0.0;
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded submission to completion metric: <min,max,avg> = <" << met.min << "," << met.max << "," << met.avg << ">");

		res.push_back(met);
	}
}

/*
 * Used to compute Archived and Retrieved metrics
 */
void DailyMetricsHandler::loadArchivedAndRetrievedMetrics(std::vector<ArchivedMetrics>& archres, std::vector<RetrievedMetrics>& retres, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	std::stringstream ss;
	ss << "SELECT producttype, platformshortname, platformserialidentifier,\n";
	ss << "       SUM(archivesize)::int8 as archive_size,\n";
	ss << "       SUM(archivecount)::int8 as archive_count,\n";
	ss << "       SUM(retrievesize)::int8 as retrieve_size,\n";
	ss << "       SUM(retrievecompletecount)::int8 as retrieve_complete,\n";
	ss << "       SUM(retrievefailcount)::int8 as retrieve_failed\n";
	ss << "FROM t_srv_totalmetrics tst\n";
	ss << "GROUP BY tst.producttype, tst.platformshortname, tst.platformserialidentifier";
	
	// force SQL query
	SQLString sql(dbKey, SQLString::COMMAND);
	sql.forceSQLString(ss.str());
	
	dbQuery query(conn);
	query.sqlString(sql, "LoadArchivedAndRetrieveMetrics");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query:\n" << query.sqlString());
	
	dbSet set;
	query.execForSet(set);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << set.size() << " records");
	
	// Load the metrics
	dbRow row(dbKey);
	while (set.getNextRow(row)) {
		int i = 0;
		
		ArchivedMetrics archived; 
		RetrievedMetrics retrieved;
		
		// Load archived
		row.column(i++, archived.prodType);
		row.column(i++, archived.platformShortName);
		row.column(i++, archived.platformSerialId);
		row.column(i++, archived.size);
		row.column(i++, archived.count);
		
		// Load retrieved
		retrieved.prodType = archived.prodType;
		retrieved.platformShortName = archived.platformShortName;
		retrieved.platformSerialId = archived.platformSerialId;
		row.column(i++, retrieved.size);
		row.column(i++, retrieved.completed);
		row.column(i++, retrieved.failed);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded archive metric: " << archived.prodType << "." << archived.platformShortName << "." << archived.platformSerialId << " -> <size,count> = <" << 
												archived.size << "," << archived.count << ">");
												
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded retrieved metric: " << retrieved.prodType << "." << retrieved.platformShortName << "." << retrieved.platformSerialId << " -> <size,completed,failed> = <" << retrieved.size << 
												"," << retrieved.completed << "," << retrieved.failed << ">");

		archres.push_back(archived);
		retres.push_back(retrieved);
	}
}

/*
 * Used to compute Archived metrics independently by the product type
 */
void DailyMetricsHandler::loadProdTypeArchivedMetrics(std::vector<ArchivedMetrics>& archres, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	std::stringstream ss;
	ss << "SELECT platformshortname, platformserialidentifier,\n";
	ss << "       SUM(archivesize)::int8 as archive_size,\n";
	ss << "       SUM(archivecount)::int8 as archive_count\n";
	ss << "FROM t_srv_totalmetrics tst\n";
	ss << "GROUP BY tst.platformshortname, tst.platformserialidentifier";
	
	// force SQL query
	SQLString sql(dbKey, SQLString::COMMAND);
	sql.forceSQLString(ss.str());
	
	dbQuery query(conn);
	query.sqlString(sql, "LoadProdTypeIndependentMetrics");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query:\n" << query.sqlString());
	
	dbSet set;
	query.execForSet(set);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << set.size() << " records");
	
	// Load the metrics
	dbRow row(dbKey);
	while (set.getNextRow(row)) {
		int i = 0;
		std::string ptype, pshortname, pserial;
		ArchivedMetrics archived;
		
		// Load archived
		row.column(i++, archived.platformShortName);
		row.column(i++, archived.platformSerialId);
		row.column(i++, archived.size);
		row.column(i++, archived.count);
		archived.prodTypeSet = false;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded archive metric: " << archived.platformShortName << "." << archived.platformSerialId << " -> <size,count> = <" << 
												archived.size << "," << archived.count << ">");

		archres.push_back(archived);
	}
}

/*
 * Used to compute Download metrics
 */
void DailyMetricsHandler::loadDownloadMetrics(std::vector<DownloadMetrics>& res, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	std::stringstream ss;
	ss << "SELECT producttype, platformshortname, platformserialidentifier, COALESCE(servicealias, \'UNKNOWN_SERVICE_ALIAS\'::character varying),\n";
	ss << "       SUM(downloadsize)::int8 as download_size,\n";
	ss << "       SUM(downloadcompletecount)::int8 as download_complete,\n";
	ss << "       SUM(downloadfailcount)::int8 as download_failed\n";
	ss << "FROM t_srv_totalmetrics tst\n";
	ss << "GROUP BY tst.producttype, tst.platformshortname, tst.platformserialidentifier, tst.servicealias";
	
	// force SQL query
	SQLString sql(dbKey, SQLString::COMMAND);
	sql.forceSQLString(ss.str());
	
	dbQuery query(conn);
	query.sqlString(sql, "LoadDownloadMetrics");

	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute the following query:\n" << query.sqlString());
	
	dbSet set;
	query.execForSet(set);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << set.size() << " records");
	
	// Load the metrics
	dbRow row(dbKey);
	while (set.getNextRow(row)) {
		int i = 0;
		
		std::string ptype, pshortname, pserial, alias;
		DownloadMetrics met; 
		
		std::string val;
		row.column(i++, met.prodType);
		row.column(i++, met.platformShortName);
		row.column(i++, met.platformSerialId);
		row.column(i++, met.serviceAlias);		
		row.column(i++, met.size);
		row.column(i++, met.completed);
		row.column(i++, met.failed);
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded download metric: " << met.prodType << "." << met.platformShortName << "." << met.platformSerialId << "." << met.serviceAlias <<
													" -> <size,completed,failed> = <" << met.size << "," << met.completed << "," << met.failed << ">");
		
		res.push_back(met);
	}
}

/*
 * Stores OriginToPublications metrics into database
 */
void DailyMetricsHandler::storeOriginToPublicationDailyMetrics(const DateTime& ref, const std::vector<OriginToPublicationMetrics>& mets, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	const std::string& dbKey = conn.getKey();
	
	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));

	std::stringstream ssbase;
	ssbase << "INSERT INTO t_srv_metrics(name, tstamp, metrictype, gauge) VALUES ";
	
	std::stringstream ssval;
	size_t blkcount = 0;
	for (size_t idx = 0; idx < mets.size(); ++idx) {
		const OriginToPublicationMetrics& opm = mets[idx];
		
		// min
		std::stringstream ssmin; 
		ssmin << "(\'OriginToPublication.Daily.min.time." << opm.prodType << "." << opm.platformShortName << "." << opm.platformSerialId << "\',";
		ssmin << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ssmin << "\'Gauge\',";
		ssmin << opm.min << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ssmin.str());
		
		// max
		std::stringstream ssmax; 
		ssmax << "(\'OriginToPublication.Daily.max.time." << opm.prodType << "." << opm.platformShortName + "." << opm.platformSerialId << "\',";
		ssmax << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ssmax << "\'Gauge\',";
		ssmax << opm.max << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ssmax.str());

		// avg
		std::stringstream ssavg; 
		ssavg << "(\'OriginToPublication.Daily.avg.time." << opm.prodType << "." << opm.platformShortName << "." << opm.platformSerialId << "\',";
		ssavg << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ssavg << "\'Gauge\',";
		ssavg << opm.avg << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ssavg.str());
		
		blkcount += 3;
		
		ssval << ssmin.str() << "," << ssmax.str() << "," << ssavg.str();
		if (blkcount >= maxInsertBlock_ || idx == mets.size() - 1) {
			SQLString sqlIns(dbKey, SQLString::COMMAND);
			sqlIns.forceSQLString(ssbase.str() + ssval.str());
			
			dbQuery query(conn);
			query.sqlString(sqlIns, "InsertDailyOriginToPublicationMetrics");
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << query.sqlString());
			
			if (!query.execForStatus()) {
				ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + sqlIns.getSQL() + "\"Error was: " + std::string(query.errorString())));
			}
			
			ssval.str("");
			blkcount = 0;
		}
		else {
			ssval << ",";
		}
	}
}

/*
 * Stores SubmissionToCompletion metrics into database
 */
void DailyMetricsHandler::storeSubmissionToCompletionDailyMetrics(const DateTime& ref, const std::vector<SubmissionToCompletionMetrics>& mets, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();

	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	std::stringstream ssbase;
	ssbase << "INSERT INTO t_srv_metrics(name, tstamp, metrictype, gauge) VALUES ";
	
	std::stringstream ssval;
	size_t blkcount = 0;
	for (size_t idx = 0; idx < mets.size(); ++idx) {
		const SubmissionToCompletionMetrics& scm = mets[idx];
		
		// min
		std::stringstream ssmin; 
		ssmin << "(\'SubmissionToCompletion.Daily.min.time\',";
		ssmin << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ssmin << "\'Gauge\',";
		ssmin << scm.min << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ssmin.str());

		// max
		std::stringstream ssmax; 
		ssmax << "(\'SubmissionToCompletion.Daily.max.time\',";
		ssmax << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ssmax << "\'Gauge\',";
		ssmax << scm.max << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ssmax.str());

		// avg
		std::stringstream ssavg; 
		ssavg << "(\'SubmissionToCompletion.Daily.avg.time\',";
		ssavg << db::toSQL(dbKey, ref, dbConv::YearToFraction) << ",";
		ssavg << "\'Gauge\',";
		ssavg << scm.avg << ")";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storing metric:" << ssavg.str());

		blkcount += 3;
		
		ssval << ssmin.str() << "," << ssmax.str() << "," << ssavg.str();
		if (blkcount >= maxInsertBlock_ || idx == mets.size() - 1) {
			SQLString sqlIns(dbKey, SQLString::COMMAND);
			sqlIns.forceSQLString(ssbase.str() + ssval.str());
			dbQuery query(conn);
			query.sqlString(sqlIns, "InsertDailySubmissionToCompletionMetrics");
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << query.sqlString());
			
			if (!query.execForStatus()) {
				ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + sqlIns.getSQL() + "\"Error was: " + std::string(query.errorString())));
			}
			
			ssval.str("");
		}
	}
}

/*
 * Locks the input db table
 */
void DailyMetricsHandler::lockDbTable(const std::string& table, const std::string& mode, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();
	
	std::string lockCommand = "LOCK TABLE " + table + " IN " + mode + " MODE";
	
	SQLString lockQueryString(dbKey, SQLString::COMMAND);
	lockQueryString.forceSQLString(lockCommand);
	
	dbQuery query(conn);
	query.sqlString(lockQueryString, "LockTable_" + table);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << query.sqlString());
	
	if (!query.execForStatus()) {
		ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + lockQueryString.getSQL() + "\"Error was: " + std::string(query.errorString())));
	}
}

/*
 * Updates the counter metrics identified by key and returns the number of affected rows
 */
size_t DailyMetricsHandler::updateCounterMetric(const std::string& key, const DateTime& ref, unsigned long long val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();
	
	SQLString sqlUpd(dbKey, SQLString::UPDATE);
	sqlUpd.addTableList("t_srv_metrics");
	sqlUpd.addSetList("tstamp", dbConv(dbKey, db::toSQL(dbKey, ref, dbConv::YearToFraction)));
	sqlUpd.addSetList("counter", dbConv(dbKey, val));
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_srv_metrics", "name" , dbParam::EQUAL_TO , dbConv(dbKey, key)));
		sqlUpd.addANDWhereParameter(pp);
	}
	dbQuery updQuery(conn);
	updQuery.sqlString(sqlUpd, "UpdateCounterMetric");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << updQuery.sqlString());
	if (!updQuery.execForStatus()) {
		ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + sqlUpd.getSQL() + "\"Error was: " + std::string(updQuery.errorString())));
	}
	
	return updQuery.getAffectedRows();
}

/*
 * Inserts the counter metric identified by key into database
 */
void DailyMetricsHandler::insertCounterMetric(const std::string& key, const DateTime& ref, unsigned long long val, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();
	
	SQLString sqlIns(dbKey, SQLString::INSERT);
	sqlIns.addTableList("t_srv_metrics");
	sqlIns.addValuesList("name", dbConv(dbKey, key));
	sqlIns.addValuesList("tstamp", db::toSQL(dbKey, ref, dbConv::YearToFraction));
	sqlIns.addValuesList("metrictype", dbConv(dbKey, "Counter"));
	sqlIns.addValuesList("counter", dbConv(dbKey, val));
	
	dbQuery query(conn);
	query.sqlString(sqlIns, "InsertCounterMetrics");
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to execute query: " << query.sqlString());
	
	if (!query.execForStatus()) {
		ACS_THROW(dbQuery::QueryExecuteException("Query Execution error SQL command was: \"" + sqlIns.getSQL() + "\"Error was: " + std::string(query.errorString())));
	}
}

/*
 * Stores Archived metrics into database
 */
void DailyMetricsHandler::storeArchivedMetrics(const DateTime& ref, const std::vector<ArchivedMetrics>& mets, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();

	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	for (size_t idx = 0; idx < mets.size(); ++idx) {
		const ArchivedMetrics& m = mets[idx];
		
		std::stringstream sskey;
		if (getIncludeMetricTypeInArchivedMetrics()) {
			sskey << "Archived.";
		}

		if (m.prodTypeSet) {
			sskey << m.prodType << ".";
		}
		sskey << m.platformShortName << "." << m.platformSerialId;
		
		std::string countkey = sskey.str() + ".count";
		std::string sizekey = sskey.str() + ".size";
		
		// BEGIN transaction
		db::Transaction transaction(conn) ; // auto roll_back if not commit
		transaction.begin_work("upsertArchivedMetricsTrans");
		
		// LOCK the table
		lockDbTable("t_srv_metrics", "EXCLUSIVE", conn);
		
		size_t affectedRows = 0;
		
		// Try count update
		affectedRows = updateCounterMetric(countkey, ref, m.count, conn);
		if (affectedRows == 0) {
			// Insert new count metric
			insertCounterMetric(countkey, ref, m.count, conn);
		}
		
		// Try size update
		affectedRows = updateCounterMetric(sizekey, ref, m.size, conn);
		if (affectedRows == 0) {
			// Insert new size metric
			insertCounterMetric(sizekey, ref, m.size, conn);
		}

		transaction.commit();
	}
}

/*
 * Stores Retrieved metrics into database
 */
void DailyMetricsHandler::storeRetrievedMetrics(const DateTime& ref, const std::vector<RetrievedMetrics>& mets, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();

	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	for (size_t idx = 0; idx < mets.size(); ++idx) {
		const RetrievedMetrics& m = mets[idx];
		
		std::stringstream sskey; 
		sskey << "Retrieved." << m.prodType << "." << m.platformShortName << "." << m.platformSerialId;
		
		std::string sizekey = sskey.str() + ".size";
		std::string compkey = sskey.str() + ".completed";
		std::string failkey = sskey.str() + ".failed";
		
		// BEGIN transaction
		db::Transaction transaction(conn) ; // auto roll_back if not commit
		transaction.begin_work("upsertRetrievedMetricsTrans");
		
		// LOCK the table
		lockDbTable("t_srv_metrics", "EXCLUSIVE", conn);
		
		size_t affectedRows = 0;
		
		// Try size update
		affectedRows = updateCounterMetric(sizekey, ref, m.size, conn);
		if (affectedRows == 0) {
			// Insert new count metric
			insertCounterMetric(sizekey, ref, m.size, conn);
		}
		
		// Try completed update
		affectedRows = updateCounterMetric(compkey, ref, m.completed, conn);
		if (affectedRows == 0) {
			// Insert new size metric
			insertCounterMetric(compkey, ref, m.completed, conn);
		}
		
		// Try failed update
		affectedRows = updateCounterMetric(failkey, ref, m.failed, conn);
		if (affectedRows == 0) {
			// Insert new size metric
			insertCounterMetric(failkey, ref, m.failed, conn);
		}

		transaction.commit();
	}
}

/*
 * Stores Download metrics into database
 */
void DailyMetricsHandler::storeDownloadMetrics(const DateTime& ref, const std::vector<DownloadMetrics>& mets, dbConnection& conn) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	const std::string& dbKey = conn.getKey();

	// Check db schema
	const std::string dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW(dialectName != "Postgres8", DailyMetricsHandlerException("Unsupported dialect: " + dialectName));
	
	for (size_t idx = 0; idx < mets.size(); ++idx) {
		const DownloadMetrics& m = mets[idx];
		
		std::stringstream sskey; 
		sskey << "Download." << m.prodType << "." << m.platformShortName << "." << m.platformSerialId << "." << m.serviceAlias;
		
		std::string sizekey = sskey.str() + ".size";
		std::string compkey = sskey.str() + ".completed";
		std::string failkey = sskey.str() + ".failed";
		
		// BEGIN transaction
		db::Transaction transaction(conn) ; // auto roll_back if not commit
		transaction.begin_work("upsertDownloadedMetricsTrans");
		
		// LOCK the table
		lockDbTable("t_srv_metrics", "EXCLUSIVE", conn);
		
		size_t affectedRows = 0;
		
		// Try size update
		affectedRows = updateCounterMetric(sizekey, ref, m.size, conn);
		if (affectedRows == 0) {
			// Insert new count metric
			insertCounterMetric(sizekey, ref, m.size, conn);
		}
		
		// Try completed update
		affectedRows = updateCounterMetric(compkey, ref, m.completed, conn);
		if (affectedRows == 0) {
			// Insert new size metric
			insertCounterMetric(compkey, ref, m.completed, conn);
		}
		
		// Try failed update
		affectedRows = updateCounterMetric(failkey, ref, m.failed, conn);
		if (affectedRows == 0) {
			// Insert new size metric
			insertCounterMetric(failkey, ref, m.failed, conn);
		}

		transaction.commit();
	}
}

_ACS_END_NAMESPACE
