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

	$Prod: A.C.S. libPackageProcessing$

	$Id$

	$Author$

	$Log$
	Revision 1.17  2016/01/29 11:15:16  marpas
	messaging strategy slightly revised
	throw xxx is deprecated: ACS macro use enforced
	useless cast to string removed
	
	Revision 1.16  2015/02/18 09:42:48  matteo.airoldi
	nicvac: throw() to destructor.
	
	Revision 1.15  2013/11/08 12:08:14  nicvac
	Improved log.
	
	Revision 1.14  2013/11/07 17:46:39  nicvac
	Fixed Task Table downloading (null case).
	
	Revision 1.13  2013/11/07 17:27:55  nicvac
	Log improved.
	
	Revision 1.12  2013/11/07 17:15:44  nicvac
	Task table management simplified.
	
	Revision 1.11  2013/11/06 13:41:13  nicvac
	Used macro namespace
	
	Revision 1.10  2013/11/04 14:27:20  marpas
	adopting libException 5.x interface
	
	Revision 1.9  2013/10/03 15:32:13  nicvac
	acs sdk warnings processed.
	
	Revision 1.8  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.7  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.6  2013/05/16 12:43:00  nicvac
	S2PDGS-317: If task table not found, task tables are refreshed from Db.
	
	Revision 1.5  2013/03/29 17:26:31  nicvac
	S2PDGS-256: implementation.
	
	Revision 1.4  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.3  2013/03/07 17:54:04  nicvac
	SQLString for portability.
	
	Revision 1.2  2013/03/05 14:07:18  nicvac
	rowCount is deprecated
	
	Revision 1.1  2012/12/12 16:21:17  nicvac
	PackageTaskTableManager added.
	
        
*/ 

#include <PackageTaskTableManager.h>

#include <ExporterServiceResolver.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbSmartPersistent.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageTaskTableManager)


PackageTaskTableManager::PackageTaskTableManager():
	_ttIdMap(),
	_ttNameMap()
{
	static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

PackageTaskTableManager::~PackageTaskTableManager() throw () {
}

void PackageTaskTableManager::downloadTaskTables() {

	Lock lock(*this);

	//*/Download Task Tables in cache

	//Get the cache
	CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();

	//Get the task tables
	dbSet set;
	try
	{
		dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
		dbConnection& conn = pw;
        const string &dbKey = conn.getKey() ;
		try {

			//This version downloads all the task tables: management is simplier and contingencies are handled easily.
			//Old versions of this class downloaded only task tables linked to distribution rules.
			// But this needs more complex management in case of contingency.

			SQLString querySQL ( dbKey, SQLString::SELECT ) ;
			querySQL.setDistinct(true);
        	querySQL.addSelectList( "id_tasktable" );
        	querySQL.addSelectList( "name" );
        	querySQL.addTableList("t_tasktables", "tt");

			dbQuery query ( conn, querySQL, "SelTaskTable" );
			query.execForSet ( set );
		} catch (exception& ex) {
			ACS_THROW(exPackageTaskTableManagerCriticalException(ex, "Error retrieving task table from DB")) ;
		}

		//Download task tables
		dbRow tt(dbKey);
		while ( set.getNextRow(tt) ) {

			int i(0); bool isNull(false); //If Null on DB, the attribute is left with its original value
			int idf(0); string ttName(""), ttContent("");
			tt.column(i++, idf, &isNull);
			tt.column(i++, ttName, &isNull);

			dbSmartPersistent tt ( "t_tasktables", conn);

			if (tt.load( conn, idf) && (tt.isSet ("tasktable", conn) ) )
			{
				// tasktable found
				// download the blob data field

				string dwnName = File::tempname("/tmp/TaskTableLoad", ".clob"); //Temp filename
				FileGuard dwnNameGuard (dwnName);
				tt.downloadFile("tasktable", dwnName, conn) ;
				{
					std::ifstream ifs( dwnName.c_str() );
					std::stringstream buffer;
					buffer << ifs.rdbuf();
					ttContent = buffer.str();
				}

			}

			//Reserve space on cache and keep exclusive lock: only this process (and my children) can read/write these files
			// and cache cannot roll them. They will be released (cache can roll and anyone can use them) at destructor time.
			string cacheName = "PackageTaskTableManager_"+ttName+".xml";
			cacheSptr->makeSpaceReserveAndUse ( cacheName, ttContent.size() );

			string ttFilename = cacheSptr->getPath()+"/"+cacheName;

			//Fill content of exclusive locked file.
			std::ofstream ofs(ttFilename.c_str());
			ofs<<ttContent;

			TTInfo ttInfo(idf, ttName, ttFilename);

			_ttIdMap[ idf ] = ttInfo;
			_ttNameMap[ ttName ] = ttInfo;

			ACS_LOG_PRIVINFO("downloaded Task Table [id, name, filename]=["
					<<ttInfo.getIdf()<<", "<<ttInfo.getName()<<", "<<ttInfo.getFilename()<<"]. Exclusively locked on cache.");
		}
	//*/
	} catch (exception& ex) {
		ACS_THROW(exPackageTaskTableManagerCriticalException(ex, "Error retrieving task table from DB")) ;
	}

}


void PackageTaskTableManager::releaseTaskTables() {

	Lock lock(*this);

	//Release the files on cache. Remove exclusive lock: cache can roll and anyone can read/write them.
	CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();

	for ( map<int,TTInfo>::iterator it=_ttIdMap.begin(); it!=_ttIdMap.end(); ++it ) {

		cacheSptr->release( acs::File::getFileName( (it->second).getFilename() ) );

		ACS_LOG_PRIVINFO("Task Table ["
				<<(it->second).getFilename()<<"]. Released on cache.");

	}
	_ttIdMap.clear();
	_ttNameMap.clear();
}



string PackageTaskTableManager::getTTFilename(std::string const& ttName) {

	return this->getTTFilename( "name", ttName, _ttNameMap );

}

string PackageTaskTableManager::getTTFilename(int const& ttId) {

	return this->getTTFilename( "id", ttId, _ttIdMap );

}

_ACS_END_NAMESPACE
