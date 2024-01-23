// PRQA S 1050 EOF
//
// 	Copyright 1995-2021, Exprivia SpA - DADF
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.exprivia.it
//
// 	All Rights Reserved.
//
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Exprivia SpA
//
// 	$Prod: PackagerOptimiserApp Main Application $
//
// 	$Id$
//
// 	$Author$
//
//  $Log$
//  Revision 2.47  2017/11/15 12:44:37  chicas
//  some message to PRIVINFO
//
//  Revision 2.46  2017/05/02 10:43:27  tergem
//  S2PDGS-1641 log harmonization
//
//  Revision 2.45  2017/03/24 17:08:27  nicvac
//  APF-273: Dissemination MC.
//
//  Revision 2.44  2016/09/07 15:22:49  marfav
//  Removed dependency from DGFDistributor conf space
//
//  Revision 2.43  2016/09/07 14:36:16  marfav
//  removed dependency from DGF-FTP conf space
//
//  Revision 2.42  2016/09/06 10:09:09  marfav
//  CSGACQ-64
//  Added support for filtering media types that do not need to be packaged
//  (Cosmo specific behaviour)
//
//  Revision 2.41  2016/07/06 15:35:09  marfav
//  CSGACQ-84
//  New project/application based log rotation management strategy enforced
//
//  Revision 2.40  2016/04/11 13:53:07  marpas
//  adopting new db interfaces
//
//  Revision 2.39  2016/01/08 15:34:36  francesco.avanzi
//  Usage Method added
//  Some ACS_LOG_INFO messages moved to ACS_LOG_PRIVINFO as such messages where intended to be.
//  Try/catch logic implemented in getConfiguration(): rsResourceSet::NotFoundException are now wrapped and will not blow to main anymore.
//
//  Revision 2.38  2014/09/24 08:48:04  lucio.pulvirenti
//  S2PDGS-760: Exiting status set if stop signal is caught only.
//
//  Revision 2.37  2014/06/25 17:10:10  marpas
//  coding best practices applied
//
//  Revision 2.36  2014/05/13 16:22:15  lucio.pulvirenti
//  S2PDGS-760: New debug macros used.
//
//  Revision 2.35  2014/05/12 17:18:34  lucio.pulvirenti
//  Work in progress.
//
//  Revision 2.34  2014/05/12 15:59:47  lucio.pulvirenti
//  S2PDGS-760: Exiting and Critical status correctly set.
//
//  Revision 2.33  2013/09/13 07:35:35  chicas
//  loadConfiguration notifyReplacement set to false
//
//  Revision 2.32  2013/05/12 14:44:09  marpas
//  no longer master/slave check
//
//  Revision 2.31  2013/05/09 19:35:45  marpas
//  listener configuration fixed if not found
//
//  Revision 2.30  2013/03/14 15:42:46  marpas
//  connection pool concept is left to class inheriting from Application
//
//  Revision 2.29  2013/02/14 09:12:57  marpas
//  mastr discovery implementation changed
//
//  Revision 2.28  2013/02/08 10:44:32  enrcar
//  EC:: removed compilation warning in PDS_OLD_SCHEMA code
//
//  Revision 2.27  2013/02/07 15:50:11  enrcar
//  EC:: Updated to handle the new schema
//
//  Revision 2.25  2013/01/11 13:47:47  marfav
//  Addedd support to external appname definition
//
//  Revision 2.24  2013/01/08 16:57:38  marfav
//  Managed formats defined as task tables (in progress needs to be improved)
//
//  Revision 2.23  2012/09/11 10:25:33  serfol
//  added the ftp distribution with the s2pp-media format
//
//  Revision 2.22  2012/09/11 10:08:40  serfol
//  added the s2gpp-media format
//
//  Revision 2.21  2012/02/09 17:54:40  marpas
//  refactoring in progress
//
//  Revision 2.20  2010/06/15 11:05:40  marpas
//  HAS_MISSION_INCLUDES management added
//
//  Revision 2.19  2009/09/28 16:58:15  marpas
//  dbParam interface obsolescence
//
//  Revision 2.18  2009/07/13 14:19:43  enrcar
//  EC:: c++rules
//
//  Revision 2.17  2009/06/12 10:00:34  enrcar
//  EC:: Formats: multi-nods-cd and multi-nods-dvd handled
//  CVS
//
//  Revision 2.16  2009/06/05 14:32:41  danalt
//  EC:: rsa_ceos supported
//
//  Revision 2.15  2009/03/24 15:16:49  enrcar
//  EC:: c++rules
//
//  Revision 2.14  2009/02/19 12:09:05  marpas
//  exception handling improved and useless libraries removed
//
//  Revision 2.13  2009/01/22 14:22:50  enrcar
//  EC:: RemoveTransientsAtStartup. Method added.
//
//  Revision 2.12  2009/01/20 10:29:39  enrcar
//  EC:: Format single_nods_ftp added as alias of cryo_ftp (the latter becomes DEPRECATED and shouldn't be used anymore)
//
//  Revision 2.11  2008/11/26 09:24:38  danalt
//  Using ConfigurationSingleton instead of MergeConfiguration to load confspaces
//
//  Revision 2.10  2008/04/28 14:02:48  crivig
//  20 log file saved
//
//  Revision 2.9  2008/04/22 15:02:33  enrcar
//  Transactions shortened. Ref. CRY-284
//
//  Revision 2.8  2008/03/06 13:55:40  crivig
//  EC:: DGF-FTP Configuration space is also used
//
//  Revision 2.7  2008/02/13 14:26:57  enrcar
//  EC:: Report handling added
//
//


#include <PackagerOptimiserApp.h>
#include <PackagerOptimiserAppV.h>
#include <PackagerOptimiser.h>
#include <PackagerOptimiserFactory.h>
#include <PackagerOptimiserSingleton.h>

#ifdef COSMO
    #include <dbCosmoFactory.h>
#else
    #include <dbCryosatFactory.h>
#endif
#include <PackagerOptBroadcastListener.h>
#include <dbSet.h>
#include <dbMediaTypeQuery.h>
#include <dbMediaType.h>
#include <dbDistributionFormatQuery.h>
#include <dbDistributionFormat.h>
#include <dbDistributionQuery.h>
#include <dbDistribution.h>
#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbConnectionPool.h>

#include <dbQueryParameters.h>
#include <dbQuery.h>
#include <dbSmartQuery.h>
#include <dbConv.h>

#include <DateTime.h>
#include <ApplicationLock.h>
#include <rsPushNSpace.h>

#include <Filterables.h>

#include <MCFilterables.h>


using namespace std;
using namespace acs::mc;

_ACS_BEGIN_NAMESPACE(acs)

namespace {
    PackagerOptimiserAppV version  ;

    PackagerOptimiserFactory InstanceOf_PackagerOptimiserFactory ;

}


ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagerOptimiserApp)


//------------------------------------------------------------------------
//
//                          PackagerOptimiserApp
//
//------------------------------------------------------------------------
PackagerOptimiserApp::PackagerOptimiserApp(const string &appName, const string &subSystem) :
#if defined (HAS_MISSION_INCLUDES)
    PROJECT_APP(appName,subSystem),
#elif GENERIC_STATION_APP
    StationApp(appName,subSystem),
#elif  COSMO
    CosmoApp(appName,subSystem),
#else
    CryosatApp(appName,subSystem),
#endif
    _maxLogSize(),
    _optimiseRescanTime(),
    _reloadConfTime(),
    _restartTime(),
    _reportFormats(),
    _reloadConfiguration(false),
    _mainThread(pthread_self()),
    _optimiser(0),
    _mediaParams(),
#ifdef PDS_OLD_SCHEMA
    _mediaFormat(),
#endif
    _ignoredMediaTypes()
{
}



//------------------------------------------------------------------------
//
//                           ~PackagerOptimiserApp
//
//------------------------------------------------------------------------
PackagerOptimiserApp::~PackagerOptimiserApp() throw()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}


//------------------------------------------------------------------------
//
//                              getConfiguration
//
//------------------------------------------------------------------------
void PackagerOptimiserApp::getConfiguration() // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);


    // Space setup
    addSpace(ConfigurationSingleton::ConfSpace("PackagerOptimiser"), ConfigurationSingleton::NoLoad); // PRQA S 3081 3
    loadConfiguration(false);
    rsResourceSet rset  = ConfigurationSingleton::instance()->get();

    // Before reading configuration, a query on DB is required to get required media
    Application::rotateLog();

    // Query on T_MediaTypes
    {
        dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
        dbConnection &conn = pw ; // PRQA S 3050
        const string &dbKey = conn.getKey() ;

        dbMediaTypeQuery* query=dynamic_cast<dbMediaTypeQuery*>(dbQueryFactoryChain::instance()->newObject("dbMediaTypeQuery", dbKey)); // PRQA S 3081, 4412
        ACS_COND_THROW(0 == query, PackagerOptimiserAppException("Error in executing dynamic_cast for dbMediaTypeQuery")); // PRQA S 3081
        dbGenQueryScopeGuard queryRecepPolicyGuard(query);


        // Perform query.
        query->resetParameters();
        query->clear();
        query->doQuery(conn) ;

        // HERE: query contains the required media

        if (requestedStop()) { return ; }	// User wants to quit before starting the game..

        {
            rsPushNSpace PackagerOptimiserNS(rset,"PackagerOptimiser") ;

            // Get the list of the ignored media types (if any)
            rset.getArray ("IgnoredMediaTypes.MediaType", _ignoredMediaTypes);
            if ( _ignoredMediaTypes.size() )
            {
                ACS_LOG_INFO ("Found " << _ignoredMediaTypes.size() << " media types to ignore");
            }
            for (size_t idx=0; idx<_ignoredMediaTypes.size(); ++idx)
            {
                ACS_LOG_INFO ("Added the media type to the ignore list [" << _ignoredMediaTypes.at(idx) << "]");
            }


            loadConfigurationParam(rset, "OptimiseRescanTime", _optimiseRescanTime);

            loadConfigurationParam(rset, "ReloadConfigurationTime", _reloadConfTime);

            loadConfigurationParam(rset, "OptimiseRestartTime", _restartTime);

            {
                rsPushNSpace MediaTypesNS(rset,"MediaTypes") ;

                unsigned long long int mintr = 0 ;
                unsigned long long maxtr = 0 ;
                unsigned long long percentage = 0 ;
                unsigned long long secs = 0 ;
                unsigned long long secsStability = 0 ;
                string mediaType ;
                DateTime delay ;
                DateTime stabilityTime ;


                _mediaParams.clear();

                size_t q_size =  query->size() ;
                for (size_t q = 0; q < q_size; ++q)
                {
                    try {

                        mediaType = (*query)[q]->getMediaType() ;
                        maxtr = (*query)[q]->getUncompressedCapacity() ;

                        ACS_LOG_PRIVINFO("getConfiguration : Found media: " << mediaType);
                        maxtr *= 1024UL ; // Since the UncompressedCapacity is expressed in Kb // PRQA S 3084, 4400

                        {
                            unsigned int m = 0 ;
                            rsPushNSpace MediaTypeNS(rset,mediaType) ;
                            rset.getValue("ConfidencePercentage", m);
                            percentage = m ;
                            rset.getValue("MinimumTreshold", m);
                            mintr = 1024UL * m  ; // Since the Threshold is expressed in Kb // PRQA S 3084, 4400
                            rset.getValue("DueDateDelay", m);
                            secs = m ;
                            rset.getValue("StabilityTimeDelay", m);
                            secsStability = m ;
                        }

                        // Apply percentage to maxtr
                        if(percentage>100) { percentage = 100 ; } // PRQA S 4400

                        {
                            long double one=1.0 ;
                            long double zerozeroone=0.01 ;
                            long double m = ( (static_cast<long double>(maxtr))*(one-(zerozeroone*(static_cast<long double>(percentage)))) ); // PRQA S 3081 2
                            maxtr = static_cast<unsigned long long int> (m) ;
                        }
                        {
                            DateTime::JD50 tmpdelay( double(secs) / double(86400.0) ); // PRQA S 3081, 4400 2
                            delay = DateTime (tmpdelay) ; // delay is evaluated !
                        }
                        {
                            DateTime::JD50 tmpdelay( double(secsStability) / double(86400.0) ); // PRQA S 3081, 4400 2
                            stabilityTime = DateTime (tmpdelay) ; // stabilityTime is evaluated !
                        }

                           ACS_LOG_PRIVINFO("getConfiguration : >> Min Treshold: " << mintr );
                        ACS_LOG_PRIVINFO("getConfiguration : >> Max Treshold (after percentage): " << maxtr );
                        ACS_LOG_PRIVINFO("getConfiguration : >> Delay [s]: " << secs );
                        ACS_LOG_PRIVINFO("getConfiguration : >> Stability_Time [s]: " << secsStability);


                        {
                            /* Store Configuration params into _mediaParams map */

                            mediaParams mediaParameters ;

                            mediaParameters.mintr = mintr ;
                            mediaParameters.maxtr = maxtr ;
                            mediaParameters.delay = delay ;
                            mediaParameters.stabilityTime = stabilityTime ;

                            _mediaParams[mediaType] = mediaParameters ;

                               ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Saving mediaType: " << mediaType );
                        }
                    }
                    catch(rsResourceSet::NotFoundException &) {
                        ACS_LOG_WARNING(mediaType << " not properly configured: distribution packaging ignored on it") ;
                    }

                }

                ACS_COND_THROW ( query->empty(), PackagerOptimiserAppException ("NO MEDIUM TYPE FOUND on T_DistributionFormats.") ) ; // PRQA S 3081

            } //rset.popNameSpace(); // ("MediaTypes")

        } // rset.popNameSpace(); // ("PackagerOptimiser")

    } // Query on T_MediaTypes

#ifdef PDS_OLD_SCHEMA

    // Query on T_MediaFormats
    {
        dbDistributionFormatQuery* query=dynamic_cast<dbDistributionFormatQuery*>(dbQueryFactoryChain::instance()->newObject("dbDistributionFormatQuery"));
        ACS_COND_THROW(0 == query, PackagerOptimiserAppException("Error in executing dynamic_cast for dbDistributionFormatQuery")) ; // PRQA S 3081
        dbGenQueryScopeGuard queryRecepPolicyGuard(query);


        // Perform query.
        query->resetParameters();
        query->clear();
        query->doQuery() ;

        _mediaFormat.clear();

        // _mediaFormat table is filled
        for (unsigned long q=0; q < query->size(); q++)
        {
            string mediaFormat = (*query)[q]->getFormat() ;

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding format: " << mediaFormat );

            _mediaFormat.push_back(mediaFormat);
        }

        ACS_COND_THROW ( query->empty(), PackagerOptimiserAppException( "PackagerOptimiserApp::getConfiguration : NO DISTRIBUTION FORMAT FOUND on T_DistributionFormats.") ) ; // PRQA S 3081

    }   // Query on T_MediaFormats

#endif //PDS_OLD_SCHEMA


    try
    {
        PackagerOptimiser::getFormatReportMap( _reportFormats ) ;
    }
       catch(exception &e)
    {
        PackagerOptimiser::TypeReportFormats voidReportFormats ;	/* CREATE AN EMPTY VECTOR */
        _reportFormats = voidReportFormats ;	/* RESET VALUE */

           ACS_LOG_WARNING("WARNING: Unable to load ReportFormat -- Check Configuration File");

#if(0)
throw ;	/* EC:: Uncomment to emit a FATAL ERROR. Otherwise, only a Warning will be raised. */
#endif
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "method executed!");
}


//------------------------------------------------------------------------
//
//                          runPackagerOptimiserApp
//
//------------------------------------------------------------------------
void PackagerOptimiserApp::runPackagerOptimiserApp()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    // EnrCar:
    // W8111_OFF  In order to keep the main loop more readable

    bool newtables = false ;
    bool forceScan = true ;    // first time

    Timer rescanTimer ;     // Used for re-create all the lists
    Timer getConfigTimer ;  // Used to reload a new configuration

    PackagerOptBroadcastListener *listener = 0;

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "starting broadcast listener" );

    try {
        listener = new PackagerOptBroadcastListener((*Application::instance()->getDefaultConnectionPool())) ;
        listener->startListening(); // Setup Table listening....
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        ACS_LOG_WARNING("No listening to DB broadcasts (if any), there could be a latency up to: " << _optimiseRescanTime << "[s]") ;
    }
    ACS_SMARTPTR<PackagerOptBroadcastListener> listenerGuard(listener) ;


    getConfigTimer.start();

    //******************** Set restart Time ***********************************
    wantToRestart(_restartTime);


    //******************** Start main loop here ***********************************
    while ( !requestedStop() )
    {
        try
        {   // THIS IS THE TRY/CATCH FOR THE MAIN LOOP


            // Setting-up main loop
            try //If something fail just go on..
            {
                /*rotateLog() ; */  // Done anyway when Phoenix works...
                if ( _reloadConfiguration || getConfigTimer.isOlder(1000*_reloadConfTime) ) // PRQA S 3084, 4400
                {
                    // reload Configuration
                    getConfiguration();
                    try {
                        if (listener) {
                            listener->reloadConfiguration() ;
                        }
                    }
                    catch(exception &x) {
                        ACS_LOG_NOTIFY_EX(x) ;
                        ACS_LOG_WARNING("No listening to DB broadcasts (if any), there could be a latency up to: " << _optimiseRescanTime << "[s]") ;
                    }
                    getConfigTimer.start(); // reset timer. THIS LINE SHOULD FOLLOW getConfiguration();
                    _reloadConfiguration = false ;
                    forceScan = true ;
                }
            }
            catch(exception &e)
            {
                ACS_LOG_WARNING("runPackagerOptimiserApp: Problem during main loop. Caused by: " << e ); // to MCF
            }

            // Wait for new tables
            newtables=false;

            waitStopOrTimeout(1);   /* avoid looping on main cycle */
            rescanTimer.start();
            while ( (!requestedStop())&&(!newtables)&&(!forceScan)&&(!rescanTimer.isOlder(1000*_optimiseRescanTime)) ) // PRQA S 3084, 4400
            {
                waitStopOrTimeout(5); /* avoid looping */  // PRQA S 4400

 	        size_t messages_in_queue = 0; 
                if (0 != listener) {
		    const bool reset_queue (true) ;
		    listener->gotMessages(messages_in_queue, reset_queue) ; // Queue is cleared
                }
                if (messages_in_queue) {newtables=true ;  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, messages_in_queue << " broadcast(s) received.") ;
 }
            }   /* while */


            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,
                                  "Out of the waiting cycle: " << "CurrentStopMode: " << currentStopModeToString() <<
                                  " newtables: " << boolalpha << newtables <<
                                  " forceScan: " << boolalpha << forceScan );

            /* HERE IF: there are new tables, a signal, or after the RESCAN Time*/

            // This is a proper time to rotate the log
            Application::rotateLog();

            if (requestedStop())
            {
                continue; // do not enter in the doWork - stop requested
            }

            doWork();   // Perform the optimisation. Exceptions might be thrown ! (see the catch)
            forceScan = false ; // No exception received from method doWork: All work is now completed: So next time will wait for timers

        } //try for main loop
        catch(exception &e)
        {
            ACS_LOG_WARNING("runPackagerOptimiserApp: Received Exception: " << e ) ;

            forceScan = true ; // It was not possible to perform the work: Next time will check anyway
        }

    } // while( requestedStop() )


    // ******** OUT OF MAIN LOOP ******** --HERE IF: Stop Forced, or Restart time
    if (listener)
    {
    	listener->stopListening();
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DONE.");
}


//------------------------------------------------------------------------
//
//                                    doWork()
//
//------------------------------------------------------------------------
void PackagerOptimiserApp::doWork()
{

// EnrCar
// W8112_OFF  The warning on Statement depth is due to macros expansion.


    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    _reloadConfiguration = false ;



#ifndef PDS_OLD_SCHEMA
    // For each medium, the list is created and the optimiser is launched
    dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050

//	THE FOLLOWING QUERY IS IMPLEMENTED:
//		SELECT DISTINCT
//			mediatype ,
//			format ,
//			(select name from t_tasktables k where k.id_tasktable = t.format) ,
//			disseminationpackingalgo_id ,
//			(select factorykey from t_disseminationpackingalgos d where d.id_disseminationpackingalgo = t.disseminationpackingalgo_id) ,
//		FROM t_distributionitems t
//		WHERE (mediatype is NOT NULL) and (format is not null) and (disseminationpackingalgo_id is not null) and (t.packageid IS NULL) ;

    const string & dbKey = theConnection.getKey() ;
    SQLString select_query( dbKey, SQLString::SELECT ) ;

    select_query.setDistinct(true) ;

    select_query.addSelectList("mediatype");					/* mediatype as string, e.g. "FTP" */
    select_query.addSelectList("format");						/* format as ID, e.g. 54  */

    {
        /* disseminationpackingalgo as string, e.g. defaultDistribFormat  */
        SQLString nested_select( dbKey, SQLString::SELECT ) ;
        nested_select.addSelectList("name");
        nested_select.addTableList("t_tasktables", "k");
        {
            dbQueryParameters pp(dbKey) ;
            pp.addParameter(dbParam(dbKey, "k","id_tasktable" , dbParam::EQUAL_TO , "t.format" )) ;
            nested_select.addANDWhereParameter(pp) ;
          }

        select_query.addSelectList(nested_select);
    }
    select_query.addSelectList("disseminationpackingalgo_id");	/* disseminationpackingalgo as ID, e.g. 3  */

    {
        /* format as string, e.g. OneItem */
        SQLString nested_select( dbKey, SQLString::SELECT ) ;
        nested_select.addSelectList("factorykey");
        nested_select.addTableList("t_disseminationpackingalgos", "d");
        {
            dbQueryParameters pp(dbKey) ;
            pp.addParameter(dbParam(dbKey, "d","id_disseminationpackingalgo" , dbParam::EQUAL_TO , "t.disseminationpackingalgo_id" )) ;
            nested_select.addANDWhereParameter(pp) ;
          }

        select_query.addSelectList(nested_select);
    }
    select_query.addTableList("t_distributionitems", "t");

    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t","mediatype" , dbParam::ISNOTNULL )) ;
        pp.addParameter(dbParam(dbKey, "t","format" , dbParam::ISNOTNULL )) ;
        pp.addParameter(dbParam(dbKey, "t","disseminationpackingalgo_id" , dbParam::ISNOTNULL )) ;
        pp.addParameter(dbParam(dbKey, "t","packageid" , dbParam::ISNULL )) ;

        // Add the mediatype filter if applicable
        for (size_t idx=0; idx<_ignoredMediaTypes.size(); ++idx)
        {
            pp.addParameter(dbParam(dbKey, "t","mediatype" , dbParam::DIFFERENT_FROM, db::toSQL (dbKey, _ignoredMediaTypes.at(idx)))) ;
        }


        select_query.addANDWhereParameter(pp) ;
      }

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to execute query: \"" << select_query.getSQL() << "\"" ) ;

    dbQuery query(theConnection, select_query, "query_on_available_packages") ;

    dbSet set ; query.execForSet(set) ;

    size_t rows = set.size() ;
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got " << rows << " records" );

    for (size_t i=0; i<rows; ++i)
    {
        dbRow row(dbKey) ;
        set.getNextRow(row) ;

        int mediaFormatId = 0 ;
        int packingAlgoId = 0 ;
        string mediaType ;
        string packingAlgo ;
        string mediaFormat ;
        size_t column_ctr = 0 ;
        bool isNull[1024] ; // PRQA S 4403

        row.column(column_ctr, mediaType, &isNull[column_ctr]) ; column_ctr++ ;	  	/* As string */
        row.column(column_ctr, mediaFormatId, &isNull[column_ctr]) ; column_ctr++ ;	/* As ID */
        row.column(column_ctr, mediaFormat, &isNull[column_ctr]) ; column_ctr++ ;	/* As string */
        row.column(column_ctr, packingAlgoId, &isNull[column_ctr]) ; column_ctr++ ;	/* As ID */
        row.column(column_ctr, packingAlgo, &isNull[column_ctr]) ;column_ctr++ ;	/* As string */

        bool nullCheck = false ;



        for (size_t j = 0; j<column_ctr; j++)
        {
            if (isNull[j]) {
                ostringstream m ;
                m << "PackagerOptimiserApp::doWork. WARNING: Unacceptable NULL value in table t_distributionitems or in a referenced table. Skipping row: " << i ;
                ACS_LOG_TRYCATCHNOTIFY_EX(PackagerOptimiserAppException(m.str())); // PRQA S 3081

                nullCheck = true ;
                break ;
            }
        }

        if (nullCheck) { continue ;  }

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,
            "About to run createOptimiser using these parameters:\n\tmediaType: \"" << mediaType <<
            "\"\n\tmediaFormat: " << mediaFormatId << " (\"" << mediaFormat << "\")\n\tpackingAlgo: " << packingAlgoId << "  (\"" << packingAlgo << "\")") ;

        {
            const MediaParamsMap::iterator j=_mediaParams.find(mediaType) ;

            if (j == _mediaParams.end())
            {
                ostringstream m ;
                m << "PackagerOptimiserApp::doWork. Internal Error: unable to find description for media: \"" << mediaType << "\"" ;
                ACS_LOG_TRYCATCHNOTIFY_EX(PackagerOptimiserAppException(m.str())); // PRQA S 3081

                continue ;
            }

#else
    for (unsigned int i=0; i<_mediaFormat.size(); i++)
    {
        const string mediaFormat = _mediaFormat[i] ;
        for (MediaParamsMap::iterator j=_mediaParams.begin(); j!=_mediaParams.end();  j++)
        {
            const string packingAlgo="UNDEFINED" ; 				/* Unused */
            const int mediaFormatId = 0, packingAlgoId = 0 ;	/* Unused */
#endif

            PackagerOptimiser* pkgOptimiser = 0 ;
            setOptimiser(0);
            bool found = false ;

            try
            {
                if ( not requestedStop() )
                {
                    /* Create Optimiser for given MediaFormat and MediaType (if possible) */

                     ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
                        DateTime tmpDate ; string tmpDate1 ;
                        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, tmpDate.utcs(tmpDate1) <<" Creating list for medium: "
                            << j->first << " Format: " << mediaFormat );
                    ACS_CLASS_END_DEBUG

                    found = createOptimiser(mediaFormatId, mediaFormat, j, packingAlgoId, packingAlgo, pkgOptimiser) ;


                    /* Create List: */
                    if (found)
                    {
                        setOptimiser(pkgOptimiser) ;
                        pkgOptimiser->makeMainList(); // forceScan set: Create list for media


                        ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
                            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Expanding MainList for medium: " << j->first << " Format: "
                                << mediaFormat) ;
                            pkgOptimiser->dumpMainUserList();
                        ACS_CLASS_END_DEBUG


                        if (not requestedStop())
                        {
                            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Optimising list for medium: " << j->first  << " Format: " << mediaFormat) ;

                            /* Run optimiser */
                            pkgOptimiser->doOptimise();

                            while ( pkgOptimiser->processToDoList() )
                            { usleep(100) ; } // PRQA S 4400

                        }
                    }
                    else
                    {
                        /* No optimiser found */
#ifndef PDS_OLD_SCHEMA
                        ostringstream m ;
                        m << "PackagerOptimiserApp::doWork. CONFIGURATION ERROR (T_DistributionItems): unable to find description for packingAlgo: \"" << packingAlgo << "\"  PLEASE REPORT IMMEDIATELY TO ACS" ;
                        ACS_LOG_TRYCATCHNOTIFY_EX(PackagerOptimiserAppException(m.str())); // PRQA S 3081
#else
                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No optimiser defined for medium: " << j->first
                                << " Format: " << mediaFormat);
#endif

                    }   /* if found */

                }   /* if (!stop) */
            }
            catch (exception &e)
            {
                _reloadConfiguration = true ;
                ACS_LOG_INFO("doWork(): Problem when optimising medium: " << j->first << " Format: " << mediaFormat
                    << " Caused by \n" << e );
            }

            setOptimiser(0);
            if (found)
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Optimiser for medium: " << j->first << " Format: " << mediaFormat
                        << " is finished.");
                delete pkgOptimiser ;
            }
        }   // for
    }   // for


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "All Optimisers are finished. DONE.");

}


#ifdef PDS_OLD_SCHEMA

//------------------------------------------------------------------------
//
//                         createOptimiser
//
//------------------------------------------------------------------------
bool 																			/* OUT: A valid optimizer was found */
    PackagerOptimiserApp::createOptimiser(	const int& mediaFormatId,			/* IN:  The mediaFormatId */
                                            const string& mediaFormat,			/* IN:  The mediaFormat */
                                            const MediaParamsMap::iterator& j,	/* IN:  Contains the mediatype (es. "CD") and all the description parameters: size, etc. */
                                            const int& packingAlgoId,			/* IGNORED */
                                            const string& IGNORED_STRING,
                                            PackagerOptimiser* &pkgOptimiser)	/* OUT: The pointer of the optimizer, if found */
{
// EnrCar
// W8111_OFF
// The code below allows to easily add/remove new Optimisers


    bool found = false ;	/* TRUE if method could create the optimiser */
    string mediaType = j->first ;


    /*====   cryosat   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "cryosat") )
    {
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") || !cmp_nocase(mediaType, "LTO") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   cryosat   =================================================================================================*/

    /*====   s2gpp   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "s2gpp-media") )
    {
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") || !cmp_nocase(mediaType, "LTO") )
         {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItemPlusRpt") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }








    }   /*====   s2gpp   =================================================================================================*/

    /*====   goce   =================================================================================================*/
     if ( !cmp_nocase(mediaFormat, "goce") )
    {
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") || !cmp_nocase(mediaType, "LTO") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   goce   =================================================================================================*/


    /*====   multi_nods_cd   =================================================================================================*/
     if ( !cmp_nocase(mediaFormat, "multi_nods_cd") )
    {
        if ( !cmp_nocase(mediaType, "CD") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

    }   /*====   multi_nods_cd   =================================================================================================*/


    /*====   multi_nods_dvd   =================================================================================================*/
     if ( !cmp_nocase(mediaFormat, "multi_nods_dvd") )
    {
        if ( !cmp_nocase(mediaType, "DVD") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

    }   /*====   multi_nods_dvd   =================================================================================================*/


    /*====   rsa_ceos   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "rsa_ceos") )
    {
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: "
                    << mediaType << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: "
                    << mediaType << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   rsa_ceos   =================================================================================================*/


    /*====   rep_ftp   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "rep_ftp") )
    {
        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   rep_ftp   =================================================================================================*/


    /*====   envisat   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "envisat") )
    {
        /* WARNING: envisat LTA not defined yet. */
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") || !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first, \
                mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   envisat   =================================================================================================*/


    /*====   envimult   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "envimult") )
    {
        /* Envisat CD/DVD Distribution */
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") || !cmp_nocase(mediaType, "LTO") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", \
                j->first, mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   envimult   =================================================================================================*/


    /*====   cryo_lta / lta   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "cryo_lta") || !cmp_nocase(mediaFormat, "lta"))
    {
        if ( !cmp_nocase(mediaType, "CD") || !cmp_nocase(mediaType, "DVD") || !cmp_nocase(mediaType, "LTO") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: DateSize") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("DateSize", j->first,\
                 mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }

        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem" ) ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", j->first,\
                 mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   cryo_lta / lta   =================================================================================================*/


    /*====   several FTP-ONLY Formats:   =================================================================================================*/
    if ( !cmp_nocase(mediaFormat, "cryo_ftp") || !cmp_nocase(mediaFormat, "goce_ftp")
        || !cmp_nocase(mediaFormat, "csks_ftp") || !cmp_nocase(mediaFormat, "goceslip")
        || !cmp_nocase(mediaFormat, "single_nods_ftp") )
    {
        if ( !cmp_nocase(mediaType, "FTP") )
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Format: " << mediaFormat <<  " MediaType: " << mediaType
                    << " Creating optimiser: OneItem") ;

            found = true ;
            pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", \
                j->first, mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
        }
    }   /*====   several FTP-ONLY Formats   =================================================================================================*/


    // if not found use the default single_nods_ftp for any kind of media types
    if ( !found && !cmp_nocase(mediaType, "FTP") )
    {
        found = true ;
        pkgOptimiser = PackagerOptimiserSingleton::instance()->newObject("OneItemPlusRpt", \
            j->first, mediaFormatId, mediaFormat, packingAlgoId, (j->second).mintr, (j->second).maxtr, (j->second).delay, (j->second).stabilityTime, _reportFormats );
    }


    return found ;
}	/* createOptimiser */


#else


//------------------------------------------------------------------------
//
//                         createOptimiser
//
//------------------------------------------------------------------------
bool 																			/* OUT: A valid optimizer was found */
    PackagerOptimiserApp::createOptimiser(	const int& mediaFormatId,			/* IN:  The mediaFormatId */ // PRQA S 4211
                                            const string& mediaFormat,			/* IN:  The mediaFormat */
                                            const MediaParamsMap::iterator& j,	/* IN:  Contains the mediatype (es. "CD") and all the description parameters: size, etc. */
                                            const int& packingAlgoId,			/* IN:  Contains the disseminationPackingAlgo, as Id */
                                            const string& packingAlgoKey,		/* IN:  Contains the disseminationPackingAlgo, as string (the string value factorkey from table t_disseminationpackingalgos) */
                                            PackagerOptimiser* &pkgOptimiser)	/* OUT: The pointer of the optimizer, if found */
{
// EnrCar
// The code below allows to easily add/remove new Optimisers

      bool found = false ;
    const string mediaType = j->first ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called for mediaFormat: "
            << mediaFormat << "  mediaType: \"" << mediaType << "\"  packingAlgoKey: \"" <<  packingAlgoKey << "\"") ;

    try
    {
        pkgOptimiser =
            PackagerOptimiserSingleton::instance()->
                newObject(packingAlgoKey,
                          j->first,
                          mediaFormatId,
                          mediaFormat,
                          packingAlgoId,
                          j->second.mintr,
                          j->second.maxtr,
                          j->second.delay,
                          j->second.stabilityTime,
                          _reportFormats );

        found = true ;
    }
    catch (PackagerOptimiserSingleton::KeyNotFound &e)	/* Other exceptions: -> throw */
    {
        ACS_LOG_NOTIFY_EX(e) ;
        found = false ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished, returning: " << boolalpha << found );


    return found ;
}	/* createOptimiser */


#endif


//------------------------------------------------------------------------
//
//                         removeTransientsAtStartup
//
//------------------------------------------------------------------------
void
        PackagerOptimiserApp::removeTransientsAtStartup() // PRQA S 4211
{

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string & dbKey = theConnection.getKey() ;
    // DistributionQueue
    dbDistributionQuery * queryDistQueue=dynamic_cast<dbDistributionQuery*>(dbQueryFactoryChain::instance()->newObject("dbDistributionQuery", dbKey)); // PRQA S 3081, 4412
    ACS_COND_THROW( 0 == queryDistQueue, PackagerOptimiserAppException("Error executing dynamic_cast for dbDistributionQuery")) ; // PRQA S 3081
    dbGenQueryScopeGuard queryDistQueueGuard(queryDistQueue);


    /* This method is executed only once, before starting. 10 Attempts will be performed.*/
    for (int c=0; (c<10) && !requestedStop(); c++) // PRQA S 4400
    {
        try
        {
            //  Query on T_DistributionQueue
            ////////////////////////////////////////////////////////////////////////////
            queryDistQueue->clear();
            queryDistQueue->resetParameters();

            dbQueryParameters DistQueueParameters(dbKey);
            DistQueueParameters.addParameter( dbParam(dbKey, "t_distributionqueue","status", dbParam::EQUAL_TO, db::toSQL(dbKey,"TRANSIENT")) ) ;


            queryDistQueue->addParameter(DistQueueParameters);
            queryDistQueue->doQuery(theConnection);	// run  query

            size_t q_size = queryDistQueue->size() ;
            for (unsigned long int q = 0; (q < q_size) && !requestedStop(); ++q)
            {
                dbPersistent::IdType packageId = (*queryDistQueue)[q]->getPackageId() ;


                   ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Processing " << "TRANSIENT" << " package " << (q+1) // PRQA S 3084
                        << "/" << queryDistQueue->size() << " Id: " << packageId ) ;


                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Detaching all items with Id: " << packageId ) ;

                dbQueryParameters itemsParameters(dbKey);
                itemsParameters.addParameter(dbParam(dbKey, "t_distributionitems","packageid", dbParam::EQUAL_TO, db::toSQL(dbKey,packageId))) ;

                  SQLString updateString( dbKey, SQLString::UPDATE ) ;
                updateString.addTableList( "T_DistributionItems" ) ;
                updateString.addSetList("PackageId", "NULL") ;

                updateString.addANDWhereParameter(itemsParameters) ;


                dbQuery updateItems(theConnection, updateString, "UPD T_DistributionItems") ;

                updateItems.execForStatus() ;


                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Done. Affected items: " << updateItems.getAffectedRows());


                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Removing DistributionQueue with Id: " << packageId) ;

                dbQueryParameters distrQueueParameters(dbKey);
                distrQueueParameters.addParameter(dbParam(dbKey, "t_distributionqueue","packageid", dbParam::EQUAL_TO, db::toSQL(dbKey,packageId))) ;

                SQLString sqlString( dbKey, SQLString::DELETE ) ;
                sqlString.addTableList( "T_DistributionQueue" ) ;
                sqlString.addANDWhereParameter(distrQueueParameters) ;

                dbQuery deleteItems(theConnection, sqlString, "DEL T_DistributionQueue") ;

                deleteItems.execForStatus() ;


                ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Affected rows: [should be: 1] " << deleteItems.getAffectedRows() );

                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Successfully removed package: " << packageId) ;

            }	/* for q */
        }
        catch(exception& e)
        {
            ostringstream m ;
            m << "PackagerOptimiserApp::removeTransientsAtStartup(). Got exception during iteration #" << c << "\n" ;
            ACS_LOG_TRYCATCHNOTIFY_EX(PackagerOptimiserAppException(m.str())) ; // PRQA S 3081

            waitStopOrTimeout (5000+(30000*c)); // PRQA S 3000, 4400
            continue ;	/* into "for c" */
        }

        break ;	/* quit "for c" -- since operation was successfully */
    }	/* for c */


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Finished." );

    return ;
}

int PackagerOptimiserApp::usage(const std::string &) const
{
    excerr << "Use: \n" ;
    excerr << "\t" << applicationName() << "\n" ;


    return 1 ;
}

//------------------------------------------------------------------------
//
//                                   main
//
//------------------------------------------------------------------------
int PackagerOptimiserApp::main(int argc, char const * const * argv, char const * const * env)	// PRQA S 4020
{

// EnrCar::
// W8115_OFF
// This method has multiple returns in order to keep its complexity
// as low as possible


    //Lock the application.
    ApplicationLock appLock;
    appLock.lock(true); //exception is raised if lock fails

    //APF 273
    MCParameters::instance()->setDefaultFunction ("Data Dissemination");
    MCParameters::instance()->setDefaultCode ("DD");


    //******************** Setup Log ****************************************
//	Application::rotateLog() ; // open the first log

    // 1st Time: Apply signature
    DateTime curDateTime ; string tmpCurDate ;
    ACS_LOG_PRIVINFO("***********  Logfile for Application " << applicationName() << ". Created on: " << curDateTime.utcs(tmpCurDate)
        << "  ***********") ;
    ACS_LOG_PRIVINFO("Filename: " << getLRPolicy().getCurrentLogName() );


    daemonize(); // LET HERE: troubles will be logged.


    // Start getting the configuration
    unsigned int loadRetry = 0;
    bool loaded = false;
    while(!requestedStop() && !loaded) // PRQA S 4236
    {
        try
        {
            // get configuration
            getConfiguration();
            Application::rotateLog() ; // open the first log
            loaded = true;  // HERE IF: successfull, so break the loop
            continue ;  // break the loop
        }
        catch (exception &e)
        {
            if (ACS_CLASS_GET_DEBUG() == 1357) { // PRQA S 4400
                ACS_LOG_WARNING("Cannot load configuration. proceed anyway " );
                loaded = true ;
                continue ;
            }
            ACS_LOG_INFO("Main Problem loading configuration. Caused by " << e << "\nKeep Trying");
        }

        // Try again
        loadRetry++;

        if(loadRetry >= 10) // PRQA S 4400
        {
            Application::setStatus (Application::Critical, "Cannot load configuration");
            ACS_THROW(exCriticalException( "Problem loading configuration. Maximum number of retries reached.")) ; // PRQA S 3081
        }

        waitStopOrTimeout(5000); // PRQA S 4400

    } // end while


    // HERE IF: Configuration loaded successfully


    if (requestedStop()) {
        return 0;   // User wants to quit before starting the game...
    }

    try
    {
        // remove packages in transient state
        removeTransientsAtStartup();

        // run application
        runPackagerOptimiserApp();

        /* HERE IF: Application exited. Main finished... */

    }   /* try */
    catch (exception & e)
    {
           Application::setStatus (Application::Critical, "Error due to exception, see log");
        ACS_THROW(exCriticalException( e, "exception caught")); // PRQA S 3081
    }


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "OUT OF THE MAIN LOOP. CurrentStopMode is: " << currentStopModeToString() );


    if (PhoenixRestart == getCurrentStopMode())
    {
        ACS_LOG_INFO("Time to restart! Application will restart in 5''..." ) ;
        waitStopOrTimeout(5000); // PRQA S 4400
        return 0;
    }
    else if (CleanStop == getCurrentStopMode() || ImmediateStop == getCurrentStopMode())
    {
        return 0;   // User wants to quit
    }
    else
    {
        // HERE IF: No restart, neither user wants to quit -- SO, AN ERROR OCCURRED!!!!!!!!!!
        ACS_LOG_ERROR("ERROR: No restart, neither user wants to quit. Application will exit in 60''...") ;
        Application::setStatus (Application::Critical);
        waitStopOrTimeout(60000);  // PRQA S 4400
        if (requestedStop()) { // can occur meanwhile
            return 0;   // User wants to quit
        }
        return -1;
    }

    /* PackagerOptimiserApp::main. DONE. */
}


void PackagerOptimiserApp::stop()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    Application::setStatus (Application::Exiting);
}


void PackagerOptimiserApp::customSigTermHandler()  // PRQA S 4020
{
    if (pthread_self() != _mainThread) {
        return ;
    }
    ACS_LOG_INFO("SigTerm caught" );
    ACS_LOG_INFO("Stop scheduled" );

    try {
        stop();	// Stop main
        stopOptimiser() ;   // If a optimiser is running....
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}


void PackagerOptimiserApp::customSigIntHandler() // PRQA S 4020
{
    if (pthread_self() != _mainThread) {
        return ;
    }
    ACS_LOG_INFO("SigInt caught" );
    ACS_LOG_INFO("Stop scheduled") ;

    try {
        stop();	// Stop main (ONLY)
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
}


void PackagerOptimiserApp::loadConfigurationParam(rsResourceSet &set, string const & tag, string & value )
{
    try{ set.getValue(tag,value); }
    catch(exception &e)
    {
        ACS_LOG_WARNING("Error while trying load parameter " << tag << " in configuration, keeping old value: " << value << "\n" << e );
    }
}


void PackagerOptimiserApp::loadConfigurationParam(rsResourceSet &set, string const & tag, unsigned int & value )
{
    try{ set.getValue(tag,value); }
    catch(exception &e)
    {
        ACS_LOG_WARNING("Error while trying load parameter " << tag << " in configuration, keeping old value: " << value << "\n" << e );
    }
}


void PackagerOptimiserApp::setOptimiser(PackagerOptimiser* p) // PRQA S 4121
{ _optimiser=p; }


void PackagerOptimiserApp::stopOptimiser()
{ if (_optimiser) { _optimiser->stop(); _optimiser=0; } }

namespace {
//
// Instancing the Class or the Application has not been built.
//

#ifndef PACKAGEROPTIMISER_SUBSYSTEM
#define PACKAGEROPTIMISER_SUBSYSTEM "SDF"
#endif

#ifndef PACKAGEROPTIMISER_APPNAME
#define PACKAGEROPTIMISER_APPNAME "PackagerOptimiser"
#endif

Application::GenericBuilder<PackagerOptimiserApp> PackagerOptimiserAppBuilder(PACKAGEROPTIMISER_APPNAME ,PACKAGEROPTIMISER_SUBSYSTEM);
}

_ACS_END_NAMESPACE
