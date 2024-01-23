/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.3  2017/04/07 10:42:04  marfav
	APF-412
	Inserting multiple rows in the waiting orders table if needed
	using a single transaction
	
	Revision 3.2  2017/02/02 16:15:42  marfav
	CSGACQ-110
	Allowing expiration date despite the trigger type value
	
	Revision 3.1  2017/01/11 16:58:45  marfav
	S3MPM-865
	In case the insert in the t_waitingorders fails the TL order is put in error
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.16  2016/10/11 07:55:06  marfav
	S2MPM-842
	CacheManager is now a pointer
	
	Revision 2.15  2016/07/12 15:00:02  marfav
	Fixed connection management in waiting orders handling
	
	Revision 2.14  2016/04/11 13:45:45  marpas
	adopting new db interfaces
	
	Revision 2.13  2016/03/29 17:16:35  marpas
	adopting new db I/F
	
	Revision 2.12  2015/05/04 15:30:27  marfav
	Removing compiler warnings
	
	Revision 2.11  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.10  2013/11/19 17:50:24  marpas
	TaskTable has private copy ctor & operator=
	
	Revision 2.9  2013/03/14 15:05:39  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.8  2013/03/11 12:21:49  marfav
	Using ConnectionPool
	
	Revision 2.7  2013/02/26 16:36:24  marfav
	Some compiler warning removed
	
	Revision 2.6  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.5  2010/03/23 17:53:06  marfav
	Added expiration date support to queries and symbol table
	
	Revision 2.4  2009/09/10 14:44:16  marfav
	Using different entries in the SymTable for the file to download and
	the file to be used in the time intervals
	
	Revision 2.3  2008/11/07 15:42:09  marfav
	_deltaT parameter passed to the preconditionchecker
	
	Revision 2.2  2008/08/01 15:38:29  marfav
	PreConditionChecker plugins framework added
	
	Revision 2.1  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.28  2006/01/19 18:05:58  marfav
	Added support for mapping alternatives instead of filetypes when downloading inputs
	
	Revision 1.27  2005/06/30 12:37:28  marfav
	Download errors now notified on excmcf
	
	Revision 1.26  2005/03/09 09:30:43  marfav
	Order queue management changed
	now the orders are put in a waitlist when precondition fails
	the database will resume the waiting orders when the right input is inventoried
	
	Revision 1.25  2005/03/03 11:49:09  marfav
	Precondition check queries now use only one db connection per order
	
	Revision 1.24  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	
	Revision 1.23  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.22  2004/10/19 08:17:07  marfav
	Support for orders satellite and mission fields added
	
	Revision 1.21  2004/08/03 14:01:52  marfav
	Align to new version of libTask using TimeRange instead of dbGeoTimeRange
	
	Revision 1.20  2004/05/04 16:23:44  marfav
	Using Thread::nanosleep instead of ::nanosleep
	
	Revision 1.19  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.18  2003/09/23 17:48:37  envisat
	fixed problem with check in verbose mode
	
	Revision 1.17  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.16  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time
	
	Revision 1.15  2003/07/18 14:50:45  marfav
	tlDaemon configuration space fixed
	
	Revision 1.14  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError
	
	Revision 1.13  2003/06/06 10:07:33  marfav
	DOWNLOAD_INPUT_FILES_STATUS fixed
	
	Revision 1.12  2003/05/13 08:45:19  marfav
	Fixed memory leak at cleanup
	
	Revision 1.11  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.10  2003/04/18 16:01:38  fracar
	aligning to new compiler version
	
	Revision 1.9  2003/04/18 15:56:56  fracar
	aligning to new compiler version
	
	Revision 1.8  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.7  2003/03/26 09:56:11  marfav
	Robustness improved
	
	Revision 1.6  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.5  2003/02/10 18:50:19  fracar
	prepared for multi-interval management
	
	Revision 1.4  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.3  2003/02/04 14:11:17  marfav
	Using ConfigurationSingleton instead of parent config
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#include <InputResourceMgr.h>

#include <TlStatusMgrSingleton.h>
#include <Filterables.h>
#include <ConfigurationSingleton.h>

#include <ThinLayer_PCC.h>
#include <PreconditionCheckerFactoryChain.h>

#include <dbConnection.h>
#include <dbWaitingOrder.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>

#include <SQLString.h>
#include <dbQuery.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <Transaction.h>

#include <algorithm>
#include <numeric>


using namespace std;
using namespace acs;

namespace {

    const char *_deltaTKey = "TlDaemon.TLD_Time_Window_Delta_T";

}

ACS_CLASS_DEFINE_DEBUG_LEVEL(InputResourceMgr)

InputResourceMgr::InputResourceMgr (const StringKey& aKey, const tlOrder& anOrder, const TaskTable& tt, CacheManager* cm) :
	_downloaders(),
	_aborted(false),
	_theKey (aKey),
	_order (anOrder),
	_theTT (tt.clone()),
	_cacheManager (cm),
	_precondChecker(NULL),
	_symbolTable(),
	_downloadedSymbolTable(),
	_deltaT ()
{

  // retrieve the delta time for time windows to be used in db queries
  _deltaT = retrieveDeltaT();
}

bool
InputResourceMgr::getVisitedTaskTable(TaskTable& tt) const
{
	// fill the passed task table with the PCC navigated one (if any)
	if (_precondChecker)
	{
		_precondChecker->getVisitedTaskTable(tt);
		return true;
	}
	else
	{
		// The precondition checker has never been created
		// do not touch the parameter and return false
		return false;
	}
}

bool
InputResourceMgr::getSymbolTable (PCC_Base::SymTable& st) const
{
	// fill the passed symbol table with the PCC one (if any)
	if (_precondChecker)
	{
		_precondChecker->getSymbolTable(st);
		return true;
	}
	else
	{
		// The precondition checker has never been created
		// do not touch the parameter and return false
		return false;
	}
}

bool
InputResourceMgr::getDownloadedSymbolTable (PCC_Base::SymTable& st) const
{
	// fill the passed symbol table with the PCC one (if any)
	if (_precondChecker)
	{
		st=_downloadedSymbolTable;
		return true;
	}
	else
	{
		// The precondition checker has never been created
		// do not touch the parameter and return false
		return false;
	}
}


InputResourceMgr::~InputResourceMgr () throw() 
{
    delete _precondChecker;
}




class irm_sym_table_element_mapper
{
public:
	irm_sym_table_element_mapper (InputResourceMgr* theMgr) :
		_mgr(theMgr)
	{}
	
	void operator() (pair<const InputAlternative, PCC_Base::SymTableParams>& p)
	{
		if (p.second.checkPassed && p.second.toBeDownloaded)
		{
			// Mapping the single alternative
			_mgr->mapAlternative(p);
		}
	}

private:
	InputResourceMgr* _mgr;
};

void
InputResourceMgr::mapAlternatives()
{
	// Create a downloader for each input alternative to be downloaded
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Mapping Alternatives to be downloaded");
	
	// clear the downloaders
	_downloaders.clear();

	// and then create the map
	for_each (_symbolTable.begin(), _symbolTable.end(), irm_sym_table_element_mapper(this) );
}

void
InputResourceMgr::mapAlternative(pair<const acs::InputAlternative, PCC_Base::SymTableParams>& element)
{
	// create a downloader and put it in a map associated to the Input Alternative

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Mapping Alternative " << element.first.getFileType() );

	_downloaders[element.first] = InputDownloader(element.second, _cacheManager);
}

void 
InputResourceMgr::updateSymbolTable (const InputAlternative& a, const acs::PCC_Base::SymTableParams& p)
{
	_downloadedSymbolTable[a] = p;
}

/**
*	Helper class needed to dump the SymbolTable structure - BEGIN
**/

class IRM_SymTableElementDumper
{
public:
	IRM_SymTableElementDumper(bool mode = false) : _blackListMode (mode)
	{}
	
	void operator() (pair<const InputAlternative, PCC_Base::SymTableParams> const& element)
	{
		if (!_blackListMode)
		{
			if (element.second.checkPassed && element.second.toBeDownloaded)
			{
				for_each (element.second.fileList.begin(), element.second.fileList.end(), *this);
			}
		}
		else
		{
			if (!element.second.checkPassed && element.second.toBeDownloaded)
			{
				cout << "- [" << element.first.getFileType() << ", " << element.second.queryTrigger << "]\n";
			}
		}
	}

	void operator() (PCC_Base::InputFileName const& element)
	{
		if (!element.fileName.empty())
			cout << "- [" << element.fileName << ", " << element.fileVersion <<"]\n";
	}

	static void dumpSymbolTable (PCC_Base::SymTable symbolTable)
	{
		excout << "IRM_SymTableElementDumper: SYMBOL TABLE DUMP BEGIN\n";
		PCC_Base::SymTable::iterator it;
		size_t j;
		for (j=1, it=symbolTable.begin(); it!=symbolTable.end(); ++j,++it)
		{
			excout << "Alternative #" << j << " " <<
				"[" << (it->first).getFileType() << "," <<
				(it->first).getFileClass() << "," <<
				InputOrigin2String((it->first).getOrigin()) << "," <<
				(it->first).getRetrievalMode() << "," <<
				(it->first).getT0() << "," <<
				(it->first).getT1() << "," <<
				FileNameType2String((it->first).getFileNameType()) << "]" <<
				": " << (it->second.checkPassed?"FOUND!":"not found") << "\n";
			excout << "\t- ToBeDownloaded = " << (it->second.toBeDownloaded?"TRUE":"FALSE") << "\n";
			excout << "\t- TriggerType = " << it->second.queryTrigger << "\n";
			excout << "\t- Filenames \n";
			for (vector< PCC_Base::InputFileName >::iterator it2 = it->second.fileList.begin();
			     it2!=it->second.fileList.end(); ++it2)
			{
				string s1, s2;
				excout << "\t\t[" << it2->fileName << ", " << it2->fileVersion << "]\n";
			}
			excout << "\t- Timeline \n";
			for (vector< PCC_Base::TimelineDetails >::iterator it2 = it->second.fileTimeline.begin();
			     it2!=it->second.fileTimeline.end(); ++it2)
			{
				if (it2->fileNameIdx == -1)
				{
					excout << "\t\t[ NO FILE ] - ";
				}
				else
				{
					string fileName = it->second.fileList[it2->fileNameIdx].fileName;
					string fileVersion = it->second.fileList[it2->fileNameIdx].fileVersion;
					excout << "\t\t[" << fileName << ", " << fileVersion << "] - ";
				}
				string s1, s2;
				excout << it2->timeRange.start().utcs(s1) << ", " << it2->timeRange.stop().utcs(s2) << "\n";
			}
			excout << "\n";
		}
		excout << "FILE MANAGEMENT SUMMARY\n";
		excout << "FILES TO BE DOWNLOADED\n";
		for_each (symbolTable.begin(), symbolTable.end(), IRM_SymTableElementDumper(false));
		excout << "FILETYPES TO BE BLACKLISTED\n";
		for_each (symbolTable.begin(), symbolTable.end(), IRM_SymTableElementDumper(true));
		excout << "IRM_SymTableElementDumper: SYMBOL TABLE DUMP END\n\n";
	}


private:
        bool _blackListMode;
};

/**
*	Helper class needed to dump the SymbolTable structure - END
**/

bool
InputResourceMgr::checkInputSet(bool verboseFlag)
{

	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB)

	// Creating the ThinLayer_PCC object and performing the check
	ACS_SMARTPTR<ThinLayer_PCC> thePcc(PreconditionCheckerFactoryChain::instance()->newObject(_theKey, _theKey));

	// The creation was ok. 
	// Let's proceed with the check
	_precondChecker = thePcc.release();

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "TL_PCC created. Starting the check")

	dbConnectionWrapper connWrp (*(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);
	bool precondResult = _precondChecker->doPreconditionCheck(*_theTT, _order, _deltaT, &conn);

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "TL_PCC check completed")
	
	// Keep track of the just computed symbol table
	_precondChecker->getSymbolTable(_symbolTable);

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			excout << "InputResourceMgr::checkInputSet: TL_PCC status message\n";
			excout << _precondChecker->getStatus() << "\n";
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				excout << "InputResourceMgr::checkInputSet: dumping TL_PCC symbol table\n";
				IRM_SymTableElementDumper::dumpSymbolTable (_symbolTable);
			ACS_CLASS_END_DEBUG
		ACS_CLASS_END_DEBUG



	if (!precondResult)
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "TL_PCC check failed. Updating the waiting orders table");
		updateOrdersWaitTable (_symbolTable, conn);
	}

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Exiting returning " << boolalpha << precondResult);

	return precondResult;
}

void
InputResourceMgr::updateOrdersWaitTable (PCC_Base::SymTable symTable, dbConnection& theConnection)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);

	PCC_Base::SymTable::iterator it;
	for (it=symTable.begin(); it!=symTable.end(); ++it)
	{
		// The blacklisted elements shall have
		// - Precondition Check status == FALSE
		// AND
		// - To Be Downloaded flag == TRUE

		if ( (it->second.toBeDownloaded) && !(it->second.checkPassed))
		{

			// APF-432
			// Remove all the existing records to avoid error caused to multiple
			// evaluation of the same order performed by different TLDaemons
			// It is safe to cleanup the table out of any transaction because
			// at any given time only one instance of the TlDaemon can keep ownership
			// of the order putting it in the PRECONDITIONCHECK status
			// No race conditions will happen during this phase
			try 
			{
				string dbKey = theConnection.getKey();
			
				SQLString deleteString(dbKey, SQLString::DELETE) ;
				deleteString.addTableList("t_waitingorders") ;

				dbQueryParameters qp(dbKey);
				qp.addParameter( dbParam( dbKey, "t_waitingorders", "orderid", dbParam::EQUAL_TO, db::toSQL(dbKey,_order.getOrderId()) ) ) ;
				deleteString.addANDWhereParameter(qp) ;

				dbQuery q(theConnection, deleteString, "WaitOrd cleanup") ;
				// execute the cleanup and in case of issues notify the problem
				if ( ! q.execForStatus() ) 
				{
					ACS_LOG_WARNING ("Got an error while cleaning up the waitingorder table for order id [" << _order.getOrderId() << "]: " << string(q.errorString()) );
				}
			}
			catch (exception& ex) 
			{
				// Ignore the exception. It is not critical. If the DB is broken the next steps will fail
				ACS_LOG_WARNING ("Unable to cleanup the waiting orders table for order id [" << _order.getOrderId() << "] due to: " << ex.what() );
			}


			// Compute the query time interval taking account of delta times and global delta margin
			DateTime startTime;
			DateTime stopTime;

			// Get start and stop time from the symbol table content (if available)
			// Otherwise use the legacy code to compute it from the order but emit a warning in the log
			if (it->second.queryStartTime.isFilled())
			{
				startTime = it->second.queryStartTime;
			}
			else
			{
				startTime = _order.getStartTime();
				startTime -= DateTime::JD50( (it->first.getT0()) / 86400.0);
				startTime += DateTime::JD50( (_deltaT) / 86400.0);

				// Emit the warning just once per run
				static bool emitWarning = true;
				if (emitWarning)
				{
					ACS_LOG_WARNING ("The PreconditionCheck plugin does not fill actual special query start time. Legacy mode used. Please check.");
					emitWarning = false;
				}
			}

			if (it->second.queryStopTime.isFilled())
			{
				stopTime = it->second.queryStopTime;
			}
			else
			{
				stopTime = _order.getStopTime() ;
				stopTime += DateTime::JD50( (it->first.getT1()) / 86400.0);
				stopTime -= DateTime::JD50( (_deltaT) / 86400.0);

				// Emit the warning just once per run
				static bool emitWarning = true;
				if (emitWarning)
				{
					ACS_LOG_WARNING ("The PreconditionCheck plugin does not fill actual special query stop time. Legacy mode used. Please check.");
					emitWarning = false;
				}
			}

			// APF-412
			// Support the insertion of multiple filetypes in the waitingorders
			// The linked ones
			vector<string> fileTypesToWait ((it->second).linkedFileTypes);
			// and ne declared in the tasktable  
			fileTypesToWait.push_back ((it->first).getFileType());

			// All the insert must be on the same transaction to avoid partial insertion
			// This will also start the transaction
			db::Transaction waitInsertTrans (theConnection);
			waitInsertTrans.begin_work ("WaitingOrder INS");
			


			// reverse iterator just to put the task table filtype first in the waiting orders queue
			for (vector<string>::const_reverse_iterator wFtIt=fileTypesToWait.rbegin(); wFtIt != fileTypesToWait.rend(); ++wFtIt)
			{

				// Insert the element in the waiting orders list
				dbWaitingOrder *dbwo= static_cast<dbWaitingOrder*>(dbPersistentFactoryChain::instance()->newObject("dbWaitingOrder", theConnection.getKey()));
				dbPersistentScopeGuard guard(dbwo);
			
				dbwo->setOrderId (_order.getOrderId());
				dbwo->setStartTime (startTime);
				dbwo->setStopTime (stopTime);
				dbwo->setTriggerType (it->second.queryTrigger);

				// This is a loop looping on the filetype iterator wFtIt
				dbwo->setFileType (*wFtIt);

				// In case of time delayed query
				// ask the query for the expiration date and set it on the waiting order record
				DateTime expirationDate = it->second.expirationDate;
				if ( expirationDate.isFilled() )
				{
					dbwo->setExpirationDate (it->second.expirationDate);
				}
	
			
				try
				{
					ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Saving record in waiting order table: " << *dbwo);
			
					dbwo->save(theConnection);
				}
				catch (exception& ex)
				{
					// S3MPM-865
					// An order that can not be inserted in the waiting list will be checked forever
					// Signal this issue with a special exception that will put the order in ERROR

					ostringstream msg;
					msg << "Unable to save record orderId [" << _order.getOrderId() << "], filetype [" << (it->first).getFileType() << "]";
					ACS_THROW ( WaitingOrderException( ex, msg.str()) );
				}
			}

			// commit the transaction 
			waitInsertTrans.commit();

		} // if element

	} // for

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Exiting");
}


class
map_element_downloader
{
	ACS_CLASS_DECLARE_DEBUG_LEVEL (map_element_downloader)
	const string& _s;
	InputResourceMgr& _mgr;

public:
	explicit map_element_downloader(InputResourceMgr& theMgr, const string& path) :
		_s(path), _mgr(theMgr)
	{
		ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	}

	void operator() (pair<const InputAlternative, InputDownloader>& _m)
	{
		ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);

		// if we've been stopped in the meantime, ignore request
		// we not notify to caller the stop request... just return
		if (_mgr.aborted())
			return;

		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "About to download");

		try
		{

			// download the files
			_m.second.download(_s);

			// and update the Input Resource Manager downloadedSymbolTable
			_mgr.updateSymbolTable (_m.first, _m.second.getSymTableParams());

			//_mgr._downloadedSymbolTable[_m.first] = _m.second.getSymTableParams();

			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Download completed");
		}
		
		catch(exception& e)
		{
			// this exception can be raised only by a download call with no valid query defined
			// we have to notify here because this exception is not handled by the downloader
			ACS_LOG_ERROR ("Error while downloading input for file type " << _m.first.getFileType() )
			ACS_LOG_NOTIFY_EX (e);
			// now we rethrow to allow the calling method to return the proper exit code
			throw;
		}
	
	}
};

ACS_CLASS_DEFINE_DEBUG_LEVEL(map_element_downloader)

bool
InputResourceMgr::downloadInputSet(const string& aDirPath)
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "mapping alternatives")

	// Map the alternatives needing the download
	mapAlternatives();

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "created downloaders [" << _downloaders.size() << "]")


	// Then execute the downloaders
	// The downloaded pathnames are stored into the downloadedSymbolTable by the helper class
	try
	{
		for_each(_downloaders.begin(), _downloaders.end(), map_element_downloader(*this, aDirPath));
	}
	catch(exception&)
	{
		// logging has been managed inside the map_element_downloader class
		// only return false to the caller
		return false;
	}
	
	// once reached this point we can have two cases:
	// 1 - the download sequence was stopped due to exit request
	// 2 - the download is complete
	
	// If the download was aborted we have to exit with the right flag
	return !aborted();
}


double
InputResourceMgr::retrieveDeltaT() const
{
  double d;
  // retrieving configuration
  const rsResourceSet& rs = ConfigurationSingleton::instance() ->get();

  if(!rs.hasKey(_deltaTKey) )    //no scheduler available
  {
	ACS_LOG_WARNING ("Cannot find " << _deltaTKey << " in the configuration file; using 0")
    d = 0.0;
  }
  else
  {
    rs.getValue(_deltaTKey, d);

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "read DeltaT " << d << " from config file")
   }

  return d;
}

inline void
_callStopDownload (pair<const InputAlternative,InputDownloader>& p)
{
    (p.second).stopDownload();
}

void
InputResourceMgr::abortDownload()
{
    _aborted = true;
    for_each(_downloaders.begin(), _downloaders.end(), _callStopDownload);
	
}

bool
InputResourceMgr::aborted() const
{
    return _aborted;
}
