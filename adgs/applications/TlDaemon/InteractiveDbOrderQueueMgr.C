/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.10  2016/05/11 11:53:57  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.9  2016/04/11 13:45:45  marpas
	adopting new db interfaces
	
	Revision 2.8  2015/05/14 08:20:49  damdec
	Compilation warning fixed.
	
	Revision 2.7  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.6  2014/02/06 09:24:00  marpas
	dir load was executed twice
	
	Revision 2.5  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.4  2013/02/26 16:36:24  marfav
	Some compiler warning removed
	
	Revision 2.3  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.2  2009/06/26 10:31:54  marpas
	no message from some class dtors
	
	Revision 2.1  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.27  2005/05/02 16:22:03  marfav
	aligned to new preHandling and postHandling methods using dbConnections
	
	Revision 1.26  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.25  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.24  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement
	
	Revision 1.23  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.22  2003/09/25 09:36:27  marfav
	Using references from ConfigurationSingleton
	
	Revision 1.21  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.20  2003/09/18 15:42:30  marfav
	Fixing the last log message about Phoenix Support
	All system calls for filesystem managing removed. Using libFile instead
	
	Revision 1.19  2003/09/18 15:36:15  marfav
	Phoenix support fixed
	
	Revision 1.18  2003/09/15 10:59:25  marfav
	Support for MMI integration added
	
	Revision 1.17  2003/07/18 14:50:45  marfav
	tlDaemon configuration space fixed
	
	Revision 1.16  2003/07/17 17:18:43  marfav
	Removed notify of exception never thrown
	
	Revision 1.15  2003/07/17 16:47:02  marfav
	Thread::runException i/f changed: fixed
	
	Revision 1.14  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError
	
	Revision 1.13  2003/07/10 14:18:32  marfav
	clone method added to exceptions
	
	Revision 1.12  2003/05/14 14:57:51  marfav
	Using auto_ptr and common code moved to OrderQueueMgr
	
	Revision 1.11  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.10  2003/04/18 16:01:38  fracar
	aligning to new compiler version
	
	Revision 1.9  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.8  2003/03/27 15:33:22  marfav
	Added plugin support and some logging improvements
	
	Revision 1.7  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support
	
	Revision 1.6  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.5  2003/02/04 17:26:17  marfav
	Update JobResponsible field
	
	Revision 1.3  2003/02/04 16:45:18  marfav
	Added registerDownloadStart method
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/



#include <InteractiveDbOrderQueueMgr.h>

#include <tlDaemon.h>

#include <RegExp.h>
#include <ConfigurationSingleton.h>
#include <dbPersistentFactoryChain.h>
#include <DateTime.h>
#include <dbOrder.h>
#include <exException.h>
#include <Filterables.h>
#include <File.h>
#include <Dir.h>
#include <rsResourceSet.h>
#include <XMLIstream.h>
#include <exStream.h>
	
#include <memory>
#include <errno.h>

#include <dbConnectionPool.h>

using namespace std;
using namespace acs;



static const char *_orderFileKey = "TlDaemon.TLD_InteractiveOrderMgr.Path";
static const char* _tlOrdersName = "order_";
static const char* _processedOrdersString = "Processed_";
static const char *_cleanedFileDir = "cleaned_up";


ACS_CLASS_DEFINE_DEBUG_LEVEL (InteractiveDbOrderQueueMgr)


// -------------------------------------------------------------------------------------
//                       CONSTRUCTOR
// -------------------------------------------------------------------------------------

InteractiveDbOrderQueueMgr::InteractiveDbOrderQueueMgr(tlDaemon& aDaemon) :
	OrderQueueMgr(aDaemon),
	_path()

{
    // log extra messages about order execution
    verbose(true);
}

// -------------------------------------------------------------------------------------
//                       DESTRUCTOR
// -------------------------------------------------------------------------------------

InteractiveDbOrderQueueMgr::~InteractiveDbOrderQueueMgr() throw() 
{
}


// -------------------------------------------------------------------------------------
//                       INIT
// -------------------------------------------------------------------------------------

void
InteractiveDbOrderQueueMgr::init()
{
  //Loading settings frome resourcespace

   const rsResourceSet &_conf = ConfigurationSingleton::instance() ->get();

   ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "init: Initializing... loading configuration parameters");

   try
   {
     _conf.getValue(_orderFileKey, _path);
     ACS_LOG_INFO("Working dir is " << _path);
   }
   catch (exception&)
   {
     ACS_LOG_WARNING("Warning in initializing InteractiveDbOrderQueueMgr: cannot find " << _orderFileKey <<
               " in the configuration file. Using default path ./");
     _path=string("./");
   }
   prepareOrdersDir();

    // instruct the daemon to log extra messages about order execution
    getParent().verbose(true);
}

class renameFiles
{
	public:
		renameFiles (const string& src, const string& dst) : _src(src), _dst(dst)
		{}

		void operator() (string name)
		{
			string source(_src+"/"+name);
			string destination (_dst+"/"+name);
			try
			{
				File::rename (source,destination);
			}
			catch (exException& e)
			{
				ACS_LOG_ERROR("An error occurred while managing the order queue. ERROR while renaming file");
				ACS_LOG_NOTIFY_EX(e);
			}
		}

	private:
		string _src, _dst;
};

void InteractiveDbOrderQueueMgr::prepareOrdersDir ()
{
	exFMessage msg;

	if (!ordersDirExists())
		ACS_THROW (OrderQueueMgrException("The orders directory \"" + _path +"\" does not exists"));

	msg << "OrderQueueMgr:: cleaning up orders dir\n";
	excout << msg;

	ostringstream destinationDir;
	vector<string> toMoveFiles;
	destinationDir << _path << "/" << _cleanedFileDir;

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "prepareOrdersDir : Creating cleaned up directory [" << destinationDir.str() << "]");


	try
	{
		File::isDir (destinationDir.str());
	}
	catch (exception&)
	{
		// the dir does not exists, it has the same name of an exisitng file or have no permission to "stat"
		try
		{
			File::mkdir (destinationDir.str());
		}
		catch (exception& e)
		{
			// the dir exists as a file or have no permission to create it
			ACS_LOG_ERROR("ERROR while creating cleaned up orders dir");
			ACS_LOG_NOTIFY_EX(e);
		}
	}

	Dir dir(_path);
	RegExp re1,re2;

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "InteractiveDbOrderQueueMgr::prepareOrdersDir : dir.load returned " << dir.size() << "elements");


	// This can be replaced by a for_each and a specialized class
	// if the Dir object inherits from vector<string>

	re1.setReg ("^" + string(_tlOrdersName) + ".*\\.xml$");
	re2.setReg ("^" + string(_processedOrdersString) + ".*\\.xml$");
	for (size_t i=0; i<dir.size(); i++)
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "prepareOrdersDir : checking [" << dir[i] << "]");

		if (re1.match (dir[i]) || re2.match (dir[i]) )
		{
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "prepareOrdersDir : added [" << dir[i] << "] to move list");
			toMoveFiles.push_back (dir[i]);
		}
	}

	for_each (toMoveFiles.begin(), toMoveFiles.end(), renameFiles (_path, destinationDir.str()) );
}

bool InteractiveDbOrderQueueMgr::ordersDirExists ()
{
	bool result = false;
	try
	{
		result = File::isDir (_path);
	}
	catch (exception &e)
	{
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_INFO("Exception caught while checking the orders dir");
			ACS_LOG_NOTIFY_EX(e); 
		ACS_CLASS_END_DEBUG
	}
	return result;
}


// -------------------------------------------------------------------------------------
//                       RETRIEVEORDERS
// -------------------------------------------------------------------------------------



void
InteractiveDbOrderQueueMgr::retrieveOrders(vector<tlOrder>& orders)
{
    orders.clear();
    // This dbKey is needed only to get an instance of the dbOrder object and is not used 
    const string dbKey = (Application::instance()->getDefaultConnectionPool())->getKey();
    ACS_SMARTPTR<dbOrder> l_order ( dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", dbKey)) );

    tlOrder anOrder;


    // reading contents of working dir

    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB,"retrieveOrders - Reading contents of working dir .... ");

	vector<string> orderFiles;
	Dir dir(_path);
	RegExp re;
	re.setReg ("^" + string(_tlOrdersName) + ".*\\.xml$");
	for (size_t i=0; i<dir.size(); i++)
	{
		if (re.match (dir[i]))
			orderFiles.push_back (dir[i]);
	}
	int resultsNumber = orderFiles.size();

	if (resultsNumber >0)
	{
		for (int i=0; i < resultsNumber; ++i)
		{
			string orderFileName = _path + "/" + orderFiles[i];
				ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "retrieveOrders - Loading order " << orderFileName);

			// loading the order
			rsResourceSet rs;
			XMLIstream istream (orderFileName);

			try
			{
				// read order from xml file
				istream >> rs;
				l_order->importObj (rs);

				// use a tlOrder for store the orders
				Order2Order ( *l_order,  anOrder);

				// rename file
				DateTime data;
				string utcs;
				utcs = data.utcs(utcs);

					ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "retrieveOrders - Order " << orderFileName <<" processed at " + utcs + "... renaming file");

				ostringstream os;
				os << _path << "/" << _processedOrdersString << utcs << "_" << orderFiles[i];
				File::rename (orderFileName, os.str() );

				anOrder.setProcessedFileName (os.str());
				orders.push_back (anOrder);
 			}

			catch (exception &e)
			{
				ACS_LOG_ERROR("ERROR while importing order " << orderFileName );
				ACS_LOG_NOTIFY_EX(e);
			}


		}
	}
}

// -------------------------------------------------------------------------------------
//                       POST HANDLING
// -------------------------------------------------------------------------------------
void  InteractiveDbOrderQueueMgr::postHandling (tlOrder& aTlOrder)
{
	string fileName = aTlOrder.getProcessedFileName();

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "postHandling - OrderId " << aTlOrder.getOrderId()
								 << " handled. Unlink file [" << fileName << "]");
	try
	{
		File::unlink (fileName);
		aTlOrder.setProcessedFileName("");
	}
	catch (exception &e)
	{
		ACS_LOG_ERROR( "ERROR while unlinking file [" << fileName << "]");
		ACS_LOG_NOTIFY_EX(e);
	}
}


// -------------------------------------------------------------------------------------
//                       GETSTATUS
// -------------------------------------------------------------------------------------
InteractiveDbOrderQueueMgr::OQM_Status  InteractiveDbOrderQueueMgr::getStatus()
{
    // _timerFreq=0 means that no timer requested or initialization problems
    // we cannot continue with no timer in Interactive mode ....

    if (this->running())
    {
	if (getTimerFrequency())
	{
	    return OQM_OK;
	}
	else
	{
	    ACS_LOG_ERROR ("ERROR - the queue manager is running but no timer set. Cannot wake up. Reporting dead QueueManager.");
	    std::exception* ex;
	    if (this->runException(ex))
	    {
	        ACS_LOG_ERROR( "OrderQueueMgr Thread: exception caugth : " << ex->what());
	        delete ex;
	    }
	    return OQM_ERROR1;
	}
    }
    else
    {
	ACS_LOG_ERROR("ERROR - the queue manager is not running");
	std::exception* ex;
	if (this->runException(ex))
	{
	    ACS_LOG_ERROR( "OrderQueueMgr Thread: exception caugth : " << ex->what());
	    delete ex;
	}
	return OQM_ERROR1;
    }
}
