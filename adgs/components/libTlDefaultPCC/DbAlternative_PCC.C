/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TlDaemon Default PCC factory library $

	$Id$

	$Author$

	$Log$
	Revision 1.22  2017/02/02 16:10:28  marfav
	CSGACQ-110
	Filling expiration date only if query declares to have an expiration date
	
	Revision 1.21  2017/01/16 13:25:48  marfav
	Enforcing cpp coding rules
	
	Revision 1.20  2016/09/16 11:50:47  lucio.pulvirenti
	PDSEV-41: where condition on satellite and / or mission is set depending on InputAlternative object status.
	
	Revision 1.19  2016/04/11 13:31:12  marpas
	adopting new db interfaces
	
	Revision 1.18  2016/03/26 21:40:37  marpas
	using new db I/F
	
	Revision 1.17  2015/06/22 12:47:17  damdec
	Compilation warnings reduced.
	
	Revision 1.16  2013/06/20 12:46:46  marfav
	Added support to {AnyFileType} value for file types declarations
	
	Revision 1.15  2013/02/21 13:52:28  marfav
	Removing compiler warnings
	
	Revision 1.14  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.13  2011/09/16 14:02:48  marfav
	Adding Stream tag support to alternatives
	
	Revision 1.12  2011/09/16 13:35:02  marfav
	Token dependency moved to a separated branch
	
	Revision 1.10  2010/03/23 17:52:14  marfav
	Added expiration date support to queries and symbol table
	
	Revision 1.9  2009/11/26 14:23:33  marfav
	Passing the optional XML section filter to the special queries
	
	Revision 1.8  2009/09/29 14:32:16  marpas
	useless private method removed
	
	Revision 1.7  2009/09/28 17:01:22  marpas
	dbParam interface obsolecence
	
	Revision 1.6  2009/09/10 13:56:47  marfav
	Now the result of the sorted query is stored in the SymTable
	in addition to the timeline coverage information
	
	Revision 1.5  2009/02/04 13:17:16  marfav
	Overlap Strategy conditioned by queries
	
	Revision 1.4  2009/01/26 18:01:46  marfav
	Taking care of null queries in case of factory exceptions
	
	Revision 1.3  2008/11/07 15:56:14  marfav
	Retrieving deltaT from internal parameter
	
	Revision 1.2  2008/09/09 15:42:56  marfav
	Indentation fixed
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
*/
/*

	Old log from previous library

	Revision 1.4  2008/07/21 09:15:52  marfav
	Log improved
	
	Revision 1.3  2008/04/30 15:57:51  marfav
	Added temporary symbol table to manage alternatives
	
	Revision 1.2  2008/04/29 12:45:39  marfav
	Added support for PreconditionCheckFactoryChain
	
	Revision 1.1.1.1  2008/04/22 13:08:32  marfav
	Importing libTlPreconditionChecker
	
	

*/

#include <DbAlternative_PCC.h>

#include <InputAlternative.h>
#include <tlOrder.h>

#include <Filterables.h>
#include <exStream.h>
#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>


#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <dbQueryFactoryChain.h>
#include <dbGeoTimeRange.h>
#include <dbInventoryObjectSpecialQuery.h>
#include <dbInventoryObject.h>
#include <dbQueryParameters.h>
#include <dbLastGeneratedOverlapStrategy.h>

#include <sstream>




using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DbAlternative_PCC)
static string _tldRootKey = "TlDaemon";
static string _disFClassCheckKey = "TLD_Disable_FileClass_Check";

DbAlternative_PCC::DbAlternative_PCC(const PCC_ParamsPtr& theParameters) : Alternative_PCC(theParameters), _disableCheck(false)
{
	// Default CTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// load configuration parameter to know if file class check has been disabled
	rsResourceSet &rs = ConfigurationSingleton::instance()->get() ;
	string key = _tldRootKey + "." + _disFClassCheckKey;
	if (rs.getValue(key, _disableCheck,false))
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, key << " configuration parameter value is " << boolalpha << _disableCheck);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, key << " configuration parameter not found. Using default: " << boolalpha << _disableCheck);
	}
}

DbAlternative_PCC::~DbAlternative_PCC() throw() 
{
	// DTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}



bool
DbAlternative_PCC::visit (InputAlternative& theAlternative)
{
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "visiting InputAlternative");



	// initialize status... will be modified if needed
	setStatus ("");

	// first, let's check if this same alternative has been already checkd
	if (isAlternativeInSymTable(theAlternative, true))
	{
	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		    excerr << "DbAlternative_PCC::visit: alternative [" <<
		    theAlternative.getFileType() << "," <<
		    theAlternative.getFileClass() << "," <<
		    InputOrigin2String(theAlternative.getOrigin()) << "," <<
		    theAlternative.getRetrievalMode() << "," <<
		    theAlternative.getT0() << "," <<
		    theAlternative.getT1() << "," <<
		    FileNameType2String(theAlternative.getFileNameType()) << "]" <<
		    " FOUND in symbol table\n";
	    ACS_CLASS_END_DEBUG

	    setCheckPassed ( getAlternativeValFromSymTable(theAlternative, true).checkPassed );
	    // done!
	    // Just update the tempSymbolTable
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "previous check was " << (isCheckPassed()?"PASSED":"NOT PASSED"));
	    addAlternativeToTempSymTable (theAlternative, getAlternativeValFromSymTable(theAlternative, true), true);

	    // don't update the _status...
	    // since the descriptor is already in the symbol table,
	    // the status has already been included before in the overall status string
	}
	else
	{
	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		    excerr << "DbAlternative_PCC::visit: alternative [" <<
		    theAlternative.getFileType() << "," <<
		    theAlternative.getFileClass() << "," <<
		    InputOrigin2String(theAlternative.getOrigin()) << "," <<
		    theAlternative.getRetrievalMode() << "," <<
		    theAlternative.getT0() << "," <<
		    theAlternative.getT1() << "," <<
		    FileNameType2String(theAlternative.getFileNameType()) << "]" <<
		    " not found in symbol table\n";
	    ACS_CLASS_END_DEBUG

	    // if never checked, do it now, and insert the alternative in the symbol table
	    SymTableParams params = check(theAlternative);
	    setCheckPassed( params.checkPassed );
	    //addAlternativeToSymTable(theAlternative, params);
	    addAlternativeToTempSymTable(theAlternative, params, true);
	    if (isCheckPassed())
        {
    		setStatus ("OK");
        }
	    else
	    {
		    ostringstream s;
		    string s1, s2;

		    tlOrder& _order = getParameters()->theOrder;
		    dbGeoTimeRange theTimeRange(_order.getStartTime(), _order.getStopTime());

		    s << "No " << theAlternative.getRetrievalMode() <<
		         " file found in DB for filetype " <<  theAlternative.getFileType() <<
		         ", fileclass " << theAlternative.getFileClass() <<
		         " over the interval [" << theTimeRange.start().utcs(s1) << " - " << theAlternative.getT0() <<
		         "," << theTimeRange.stop().utcs(s2) << " + " << theAlternative.getT1() << "]";

		    setStatus (s.str());
	    }
	}

	return isCheckPassed();
}



PCC_Base::SymTableParams
DbAlternative_PCC::check(InputAlternative& theAlternative)
{

    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checking alternative");
    const string dbKey = getParameters()->dbKey ;

	SymTableParams theResult;

	// Add the stream attribute to the results
	theResult.isStream = theAlternative.getStream();

	string theFileType = theAlternative.getFileType();
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "filetype is " << theFileType);

	double _deltaT = getParameters()->deltaT;
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_deltaT is " << _deltaT);
	bool _ok;
	vector<string> _pathnames; // full pathname of the downloaded files
	vector<pair <int, acs::dbGeoTimeRange> > _intervals;

	// The _query object will be delete exiting from this method
	// the query result is stored in the returned object
	ACS_SMARTPTR<dbInventoryObjectSpecialQuery> _query(nullptr);
	tlOrder& _order = getParameters()->theOrder;
	dbGeoTimeRange theTimeRange(_order.getStartTime(), _order.getStopTime());




	string query_name = string("dbInventory") + theAlternative.getRetrievalMode() + string("Query");


   	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "query name to download file type " <<
					theFileType << " is " << query_name);




	try
	{
	  _query.reset( dynamic_cast<dbInventoryObjectSpecialQuery*> (dbQueryFactoryChain::instance()->newObject(query_name, dbKey)) );


	  if (!_query.get())
	  {
	    ACS_LOG_ERROR("DbAlternative_PCC: ERROR unknown query type " << query_name <<
		      " specified for file type " << theFileType << "\n");

	    // There is no new query... so no need to delete it
	    // but i have to store the information that the check was not ok
	    _ok = false;
	    theResult.checkPassed = _ok;
	    return theResult;
	    //return false;
	  }

   	      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "allocated new query ...  [" <<
					    theFileType << "]\n");


	      // set the parametersCLOB
	      istringstream iss;
	      iss.str(_order.getParametersCLOB());
	      _query->setParametersCLOB(iss);

    	  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Clob Parameters passed to query is ...  [" <<
				      _query->getParametersCLOB());

	  _query->t0(theTimeRange.start());
	  _query->t1(theTimeRange.stop());
	  _query->dt0(theAlternative.getT0() - _deltaT); // account for margin tolerance
	  _query->dt1(theAlternative.getT1() - _deltaT); // account for margin tolerance

	  dbQueryParameters parameters(dbKey) ;
	  // Do not set the FileType filter in case {AnyFileType} is set as the FileType in the task table
	  if ("{AnyFileType}" != theFileType)
	  {
	      parameters.addParameter(dbParam(dbKey,"t_inventory","filetype", dbParam::EQUAL_TO, db::toSQL(dbKey,theAlternative.getFileType()))) ;
	  }
	  else
	  {
    	  	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file type filter has been disabled for this input alternative.");
	  }

	  parameters.addParameter(dbParam(dbKey, "t_inventory","validityflag", dbParam::EQUAL_TO, db::toSQL(dbKey,true))) ;

	      // Check for fileclass attribute only if the requested fileclass is not empty and the check is not disabled by configuration parameter 
	      // If no fileclass is selected no filter will be set. SUBS fileclass management has been removed
	      string theClass = theAlternative.getFileClass();
 
	      if (theClass != "" && !_disableCheck) 
	      {
		      // The file class is set
   	  	      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] "  
			      << "File Class is [" << theClass << "]");
		      parameters.addParameter(dbParam(dbKey,"t_inventory","fileclass", dbParam::EQUAL_TO, db::toSQL(dbKey,theClass) )) ;
	      }
	      else 
	      {
   	  	      // The file class is not set. Just emit a debug message. The SUBS management has been removed
   	  	      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] No fileclass requested");
	      }

	      _query->addParameter(parameters);

	// If filterBySatellite flag is set
	if( theAlternative.getFilterBySatellite() ) {
	      // If the satellite is set in the Order download files having the same satelliteId or NULL satelliteId
	      // If the satellite is set check the Mission too using the rule
	      // - if the mission is set in the Order download files having the same mission or NULL missionn
	      bool paramSet;
	      int intParam;
	      intParam = _order.getSatelliteId(&paramSet);
	      if (paramSet)
	      {
		      // The satellite is set
  	  	      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] "  
			      << "SatelliteId is " << intParam);


		      // Compose the new parameters using OR conditions
		      dbQueryParameters param1(dbKey) ;
		      param1.addParameter (dbParam(dbKey,"t_inventory","satelliteid", dbParam::EQUAL_TO, db::toSQL(dbKey,intParam)));
		      param1.addParameter (dbParam(dbKey,"t_inventory","satelliteid", dbParam::ISNULL));		
		      param1.setParamOperator (dbQueryParameters::OR);
		      _query->addParameter(param1);
			  
			  if( theAlternative.getFilterByMission() ) {
		    	  // Now check for Mission
		    	  intParam = _order.getMission(&paramSet);
		    	  if (paramSet)
		    	  {
			    	  // The Mission is set
			    	  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
					    	  << "Mission is " << intParam);

			    	  // Compose the where parameters using OR operator
			    	  dbQueryParameters param2(dbKey);
			    	  param2.addParameter (dbParam(dbKey,"t_inventory","mission", dbParam::EQUAL_TO, db::toSQL(dbKey,intParam)));
			    	  param2.addParameter (dbParam(dbKey,"t_inventory","mission", dbParam::ISNULL));		
			    	  param2.setParamOperator (dbQueryParameters::OR);
			    	  _query->addParameter(param2);
		    	  }
		    	  else
		    	  {
			    	  // No mission set... no checks on mission
			    	  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
				    	  << "Mission not set in the order: no mission, no party!");
		    	  }
			  }
			  else {
				  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
					  << "do not filter by mission!");
			  }
			  
	      }
	      else
	      {
		      // No satellite set... no checks on satellite and mission
		      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
			      << "Satellite not set in the order: no satellite, no party!");
	      }
	  }
	  else {
		  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
			  << "do not filter by satellte!");
	  }

		// Alternative Filter parameters management
		if (theAlternative.isSetFilter())
		{
			_query->setOptionalFilter (theAlternative.getFilterContent(), theAlternative.getFilterRootTag());
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
			      << " FILTER set [\n" << theAlternative.getFilterContent() << "\n" << "] with RootTag ["
			      << theAlternative.getFilterRootTag());

		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DbAlternative_PCC::[" << theFileType << "] " 
			      << " no FILTER set!");
		}



	  _query->setOrderBy(" T_Inventory.ValidityStart ASC, T_Inventory.ValidityStop ASC, T_Inventory.GenerationTime DESC ");
	  _query->doQuery(dbConnectionWrapper(db::ConnPools::instance()->getPool(dbKey))) ;

	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		      excout << "DbAlternative_PCC:: check() : dump query result as received from doQuery\n";
		      for (int i=0; i<int(_query->size()); ++i)
		      {
			      excout << "query[" << i << "] : \"" << (*_query)[i]->getFileName() << "\"\n";
		      }
	ACS_CLASS_END_DEBUG



	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		{
		 string s1, s2;
		 excout << "DbAlternative_PCC::check[" << theFileType <<
			"," << theAlternative.getFileClass() << 
			"," << theTimeRange.start().utcs(s1) <<
			"," << theTimeRange.stop().utcs(s2) <<
			"," << theAlternative.getRetrievalMode() << "]: " <<
			_query->size() << " records found in inventory to be downloaded.\n";
		}
	ACS_CLASS_END_DEBUG


      // Cleanup the result about file details
      theResult.fileList.clear();
      theResult.fileTimeline.clear();

      if ( _query->doesOverlap() ) // Overlap query
      {
	

	  // compute intervals for overlapping files over the requested interval
	  dbLastGeneratedOverlapStrategy ovSt1(*_query);

	      {
		  // use the same time interval of the query for the computeOverlap ....
		  dbGeoTimeRange extendedTimeRange ( _query->startTime(), _query->endTime() );
		  ovSt1.computeOverlap(_intervals, extendedTimeRange);

	      ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		string s1, s2, s3, s4;
		excout << "DbAlternative_PCC::check overlap strategy results\n";
		for (size_t i=0; i<_intervals.size(); ++i)
		{
			excerr << "   #" << i << " - [ " << _intervals[i].first << ": " << _intervals[i].second.start().utcs(s3) <<
			          ", " << _intervals[i].second.stop().utcs(s4) << " ]\n";
		}
	      ACS_CLASS_END_DEBUG

	      }


	      ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		string s1, s2;
		excout << "DbAlternative_PCC::check[" << theFileType <<
		       "," << theAlternative.getFileClass() << 
		       "," << theTimeRange.start().utcs(s1) <<
		       "," << theTimeRange.stop().utcs(s2) <<
		       "," << theAlternative.getRetrievalMode() << "]: " <<
		       _intervals.size() << " intervals returned by the computeOverlap call.\n";
	      ACS_CLASS_END_DEBUG


		// Fill the SymTable filelist with the result of the query
		for (size_t i=0; i<_query->size(); ++i)
		{
			InputFileName input;
			input.fileName = (*_query)[i]->getFileName();
			input.fileVersion = (*_query)[i]->getFileVersion();
			theResult.fileList.push_back(input);
		}

		// Fill the SymTable timeline with the results of the overlap strategy
		// -1 means no file
		for (size_t i=0; i<_intervals.size(); ++i)
		{
			TimelineDetails details;
			details.timeRange = _intervals[i].second;
			details.fileNameIdx = _intervals[i].first;
			theResult.fileTimeline.push_back(details);
		}
      }
      else
      {
         // the query does not overlap
         // create a fake timeline containing all the query result
         // in order to download everything and
         // let the JobOrder plugin to decide if 
         // dump the result or not at its convenience
    
         DateTime startTime=theTimeRange.start();
         DateTime stopTime=theTimeRange.stop() ;

         startTime-=DateTime::JD50( (theAlternative.getT0()) / 86400.0);
         stopTime+=DateTime::JD50( (theAlternative.getT1()) / 86400.0);
	
         dbGeoTimeRange extendedTimeRange ( startTime, stopTime );

         for (size_t i=0; i<_query->size(); ++i)
         {
             InputFileName input;
             TimelineDetails details;

             // fill the input file name structure
             input.fileName = (*_query)[i]->getFileName();
             input.fileVersion = (*_query)[i]->getFileVersion();

             details.timeRange = extendedTimeRange; // Complete coverage
             details.fileNameIdx = i;

	     // add the complete list of files to the file list and timeline
             theResult.fileList.push_back(input);
             theResult.fileTimeline.push_back(details);
         }
        
      }


	  _ok = (_query->size() > 0);

	}
	catch(exception &e)
	{
	    string s1, s2;
		ACS_LOG_ERROR("DbAlternative_PCC [" << theFileType <<
		       "," << theAlternative.getFileClass() <<
		       "," << theTimeRange.start().utcs(s1) <<
		       "," << theTimeRange.stop().utcs(s2) <<
		       "," << theAlternative.getRetrievalMode() << "]: got exception while executing query\n");

	    	ACS_LOG_NOTIFY_EX(e) ;

	    _ok = false;
	}

	theResult.checkPassed = _ok;
	if (_query.get())
	{
		// There is a valid query 
		theResult.queryTrigger = _query->getTriggerType();

		// Manage expiration date in case it exists
		if (_query->hasExpirationDate())
		{
			// Ask the query for the expiration date then
			// It is still possible at this level to return a DateTime(false) -> no expiration
			theResult.expirationDate = _query->getComputedExpirationDate();	
		}
		else
		{
			// This is actually useless but is here for sake of code readability
			// Expiration dates are DateTime (false) by default at any level
			theResult.expirationDate = DateTime(false);
		}

		// APF-412
		// This is useless but improves code readability
		theResult.linkedFileTypes.clear();
		// Get any additional file type to wait	
		_query->addLinkedFileTypesToWait (theResult.linkedFileTypes);

		// CRYO2PDS-130
		// Add information on the actual start and stop times used in the query
		theResult.queryStartTime = _query->startTime();
		theResult.queryStopTime = _query->endTime();

	}
	return theResult;
	//return _ok;
}
_ACS_END_NAMESPACE

