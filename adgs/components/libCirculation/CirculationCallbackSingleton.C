// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libCirculation$

	$Id$

	$Author$

	$Log$
	Revision 7.3  2018/06/14 14:42:24  damdec
	PDSEV-24: implementation finalized.
	
	Revision 7.2  2017/10/19 16:18:03  marpas
	getting rid of EntityBean
	
	Revision 7.1  2017/03/03 09:30:23  marfav
	S3MPM-874
	Aligned to libException Rev_5_93 new Condition Interface
	
	Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
	Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
	
	Revision 1.2  2016/08/31 15:54:08  marfav
	Added explicit DTOR with throw()
	
	Revision 1.1  2016/08/31 08:56:30  damdec
	Added.
	
        
*/ 

#include <CirculationCallbackSingleton.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbSchemaMetaDataProvider.h>
#include <HWScan.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(CirculationCallbackSingleton)



CirculationCallbackSingleton::CirculationCallbackSingleton(): 
    Singleton <CirculationCallbackSingleton>(), 
	_pendingCirculations(),
	_standbyCondition()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

CirculationCallbackSingleton::~CirculationCallbackSingleton() throw()
{
}

void CirculationCallbackSingleton::push(const CirculationInfo & info)	// push back
{
	ThreadSafe::Lock conditionLock(_standbyCondition); // PRQA S 3050
	if (_pendingCirculations.empty())
	{
		_standbyCondition.broadcast();
	}
	_pendingCirculations.push(info);
}

size_t CirculationCallbackSingleton::size() // PRQA S 4211
{
	ThreadSafe::Lock conditionLock(_standbyCondition); // PRQA S 3050
	return _pendingCirculations.size();
}

bool CirculationCallbackSingleton::empty() // PRQA S 4211
{
	ThreadSafe::Lock conditionLock(_standbyCondition); // PRQA S 3050
	return _pendingCirculations.empty();
}
	
bool CirculationCallbackSingleton::getCallbackEventsFromQueue (CirculationInfo& item)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); 
	ThreadSafe::Lock conditionLock(_standbyCondition); // PRQA S 3050
	bool notEmpty(true);
	if (!_pendingCirculations.empty())
	{
		item = _pendingCirculations.front();
		_pendingCirculations.pop();
		
	}
	else
	{
		notEmpty = false;
	}
	return notEmpty;
}
Condition & CirculationCallbackSingleton::getStandbyCondition() // PRQA S 4120
{
	return _standbyCondition; // PRQA S 4024
}

void CirculationCallbackSingleton::insertCallbackEventsInQueue (dbConnection& conn, const dbPersistent::IdType & circ_id, const short & status_id, const DateTime & updatetime)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); 
	const string &dbKey = conn.getKey() ;
	
    if (dbSchemaMetaDataProvider::instance(dbKey)->existTable("t_circ_callbacks", conn))
	{
		// now search for the values corresponding to id in the t_circ_callbacks table
		SQLString sqlString ( dbKey, SQLString::SELECT ) ;
		sqlString.addTableList("t_circ_callbacks") ;
    	sqlString.addSelectList( "t_circ_callbacks.external_id" );
    	sqlString.addSelectList( "t_circ_callbacks.url" );
		{
	    	dbQueryParameters pp(dbKey);
	    	pp.addParameter( dbParam(dbKey, "t_circ_callbacks", "circ_queue_id", dbParam::EQUAL_TO, db::toSQL(dbKey, circ_id))); // PRQA S 3050, 3081
	    	sqlString.addANDWhereParameter(pp);
    	}  
		dbQuery q(conn, sqlString, "Retrieve circulation callbacks") ;
    	dbSet set ;
		q.execForSet(set) ;
		size_t setSize = set.size();
		if (setSize > 0) // some status change to be signalled
		{
			// It should not occur
			if( setSize > 1) {
				ostringstream exMsg;
				exMsg << "Unxpected result for query: request circulation id " << circ_id;
				ACS_THROW( dbQuery::QueryExecuteException(exMsg.str()) ); // PRQA S 3081
			}
			dbPersistent::IdType external_id(0);
			string url;

			dbRow row(dbKey) ;
			set.getNextRow(row) ;
			row.column(0, external_id) ; // PRQA S 4400
			row.column(1, url) ; 

			CirculationInfo tmpInfo;
			// fill tmpInfo
			tmpInfo.status_id = Status::instance(dbKey)->name(status_id);
			tmpInfo.timestamp = updatetime;
			tmpInfo.extId = external_id;
			tmpInfo.url = url;
			tmpInfo.circId = circ_id;
			tmpInfo.hostname = HWScan::hostname();
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding in Pending Circulations queue the following info: status_id = " <<  status_id << " updatetime = "<< updatetime<< " external_id = "
				<< external_id << " url = " << url );
			// save circulation info in Pending Circulations queue
			this->push(tmpInfo);		
		}
		
	}
}

_ACS_END_NAMESPACE

