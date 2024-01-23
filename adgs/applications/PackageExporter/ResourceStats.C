/*
	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. PackageExporter$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/02/23 16:37:50  nicvac
	S3EVOL-13: Distribution working area is not related anymore to download cache.
	
	Revision 1.2  2017/02/21 17:05:57  nicvac
	S3EVOL-13: Inter process cache lock mode; cache limit check
	
	Revision 1.1  2016/12/20 11:12:56  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
        
*/ 

#include <ResourceStats.h>

#include <File.h>

#include <Status.h>
#include <Application.h>
#include <dbConnectionPool.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ResourceStats)


ResourceStats::ResourceStats(size_t pendingRequestsLimit, size_t waitingAcknowledgeLimit,
							 size_t minSpaceRequiredMb, string const& spaceRequiredPath,
							 string const& jobResponsible,
							 PackageProcessingManager* packageProcessingManagerPtr):
	_pendingRequestsLimit(pendingRequestsLimit),
	_waitingAcknowledgeLimit(waitingAcknowledgeLimit),
	_minSpaceRequiredMb(minSpaceRequiredMb),
	_spaceRequiredPath(spaceRequiredPath),
	_jobResponsible(jobResponsible),
	_packageProcessingManagerPtr(packageProcessingManagerPtr)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ResourceStats::~ResourceStats() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


bool ResourceStats::isPendingRequestsLimit() const {
	return ( _packageProcessingManagerPtr->getNumOfPendingRequests() > _pendingRequestsLimit );
}

bool ResourceStats::isWaitingAcknowledgeLimit() const {

	bool limitReached(false);
	try {

		dbSet set;
		dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
		dbConnection& conn = pw;
		const string& dbKey = conn.getKey();

		SQLString sqlString(dbKey, SQLString::SELECT);
        SQLDialect::SQLCast sqlCast(SQLDialect::SQL_BIGINT) ;
		sqlString.addSelectList("COUNT(*)", sqlCast);
		sqlString.addTableList("t_distributionqueue", "disq");
		dbQueryParameters pp(dbKey);
		pp.addParameter( dbParam(dbKey, "disq","status", dbParam::EQUAL_TO, db::toSQL(dbKey, acs::Status::waitingack)) );
		pp.addParameter( dbParam(dbKey, "disq","jobresponsible", dbParam::EQUAL_TO, db::toSQL(dbKey,_jobResponsible)) );
		sqlString.addANDWhereParameter( pp );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<sqlString.getSQL() );

		dbQuery query(conn, sqlString, "DistQueueCheckWaitAckLimit");
		query.execForSet( set );

		size_t countWaitingAck(0);
		dbRow row(dbKey); set.getRow(1, row);
		row.column(0, countWaitingAck);

		limitReached = countWaitingAck >= _waitingAcknowledgeLimit;

	} catch( std::exception& ex ) {
		//Just log the error
		exResourceStatsException x(ex, "Cannot get current number of distribution orders in "+acs::Status::waitingack+" status");
		ACS_LOG_TRYCATCHNOTIFY_EX( x );
	}

	return limitReached;
}

bool ResourceStats::isDiskSpaceLimit() const {

	const size_t freeDiskMb = File::getFreeSpaceInMB ( _spaceRequiredPath );
	return ( freeDiskMb < _minSpaceRequiredMb );

}

_ACS_END_NAMESPACE

