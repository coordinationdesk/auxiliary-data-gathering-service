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

	$Prod: A.C.S. libDefaultDistributorCreator$

	$Id$

	$Author$

	$Log$
	Revision 2.15  2017/03/29 08:47:38  matteo.airoldi
	Added line in log
	
	Revision 2.14  2017/03/29 08:44:44  matteo.airoldi
	APF-404 fixed
	
	Revision 2.13  2017/01/19 21:28:44  matteo.airoldi
	APF-399 fixed
	
	Revision 2.12  2016/12/05 10:17:30  francesco.avanzi
	fixed flag problems
	
	Revision 2.11  2016/11/30 16:42:35  francesco.avanzi
	workInProgress
	
	Revision 2.10  2016/11/30 10:53:44  francesco.avanzi
	workInProgress
	
	Revision 2.9  2016/11/30 10:00:28  francesco.avanzi
	workInProgress
	
	Revision 2.8  2016/11/30 09:08:25  francesco.avanzi
	workInProgress
	
	Revision 2.7  2016/11/29 16:08:41  francesco.avanzi
	workInProgres
	
	Revision 2.6  2016/07/13 15:12:46  marfav
	CSGACQ-86 oracle sql code reviewed and fixed
	
	Revision 2.5  2016/06/01 13:02:15  lucio.pulvirenti
	APF-379: QUALOK, QUALOKVALSTOPLT, QUALOKVALSTOPGT algorithms managed to specialize query on t_inventory records accordingly.
	
	Revision 2.4  2015/09/08 14:12:32  nicvac
	Fixed query (cast)
	
	Revision 2.3  2015/08/19 12:15:30  nicvac
	APF-306: Algo QUALIFIEDALWAYS
	
	Revision 2.2  2015/07/07 12:59:11  nicvac
	APF-264: conf is in days
	
	Revision 2.1  2015/05/15 16:31:05  nicvac
	APF-264
	
        
*/ 

#include <QualifiedValiditystopDistributorCreator.h>
#include <DistributionAlgo.h>
#include <StringUtils.h>
#include <dbConnectionPool.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbColumn.h>
#include <DateTime.h>
#include <dbQueryFactoryChain.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QualifiedValiditystopDistributorCreator)



QualifiedValiditystopDistributorCreator::QualifiedValiditystopDistributorCreator(const string &algo) :
	DistributorCreator(algo),
	_threshold(1),
	_checkMode( noCheck ),
	_qualOk(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if      ( algo==QUALIFIEDVALSTOPLT ) {
		_checkMode = less;
	}
	else if ( algo==QUALIFIEDVALSTOPGT ) {
		_checkMode = greater;
	}
	else if ( algo==QUALIFIEDALWAYS ) {
		_checkMode = noCheck;
	}
	else if ( algo==QUALOK ) {
		_qualOk = true;
	}
	else if ( algo==QUALOKVALSTOPLT ) {
		_checkMode = less;
		_qualOk = true;
	}
	else if ( algo==QUALOKVALSTOPGT ) {
		_checkMode = greater;
		_qualOk = true;
	}
	else {
		ACS_LOG_WARNING("Undefined algorithm [" << algo << "]"); //Never happens as per Factory construction
	}

	ACS_LOG_INFO("Distribution Algo is [" << algo << "]");

	// get from configuration the time threshold to promote a distribution
	string key ;
	double dayThreshold(1.0);
	try	{
		getConf("TimeThreshold", key, dayThreshold, "") ;

	} catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("Cannot find configuration value for " << key << " setting default to: [" <<dayThreshold<<"] days");
	}
	_threshold = dayThreshold ;
}

QualifiedValiditystopDistributorCreator::~QualifiedValiditystopDistributorCreator() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void QualifiedValiditystopDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred) const // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &k = theConnection.getKey();

	// get the algoId from t_distributionalgo table
    int algoid = DistributionAlgo::instance(k)->id(_algo); // PRQA S 3000

// SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp,
// 	(CASE WHEN t.cartid IS NULL THEN -1 ELSE t.cartid END), 
// 	(CASE WHEN t.subreq IS NULL THEN -1 ELSE t.subreq END), 
// 	(CAST ' ' as varchar(5) ), 
// 	t.policy, t.disseminationpackingalgo_id, i.qualifierfile, i.validityStop , i.qualityinfo, q.name
// FROM
// t_temporarydistribution t
// join t_users u ON t.userid = u.userid
// join t_inventory i ON (t.filename = i.filename AND t.fileversion=i.fileversion)
// join t_qualifiedflag q ON i.qualified = q.id
// WHERE 
// t.preselection = true
// AND (t.orderid is NULL OR u.allqualified = false)
// AND t.algo = algoid

    
     
    SQLString selectString( k , SQLString::SELECT ) ;
    selectString.addSelectList("t.id") ;
    selectString.addSelectList("t.userid") ;
    selectString.addSelectList("t.mediatype") ;
    selectString.addSelectList("t.filename") ;
    selectString.addSelectList("t.fileversion") ;
    selectString.addSelectList("t.format") ;
    selectString.addSelectList("t.duedate") ;
    selectString.addSelectList("t.tstamp") ;
    {
        SQLString::SQLCase sql_case ;

	    {
		    dbQueryParameters cond1(k) ;
		    cond1.addParameter(dbParam(k, "t","cartid" , dbParam::ISNULL ) ) ;
		    sql_case.addWhen(cond1, db::toSQL(k,-1) ) ;
	    }  
        sql_case.setElse( "t.cartid" ) ; 
	    selectString.addSelectList( sql_case  ) ;
    }
    {
        SQLString::SQLCase sql_case ;

	    {
		    dbQueryParameters cond1(k) ;
		    cond1.addParameter(dbParam(k, "t","subreq" , dbParam::ISNULL ) ) ;
		    sql_case.addWhen(cond1, db::toSQL(k,-1) ) ;
	    }  
        sql_case.setElse( "t.subreq" ) ; 
        selectString.addSelectList( sql_case  ) ;
    }
    selectString.addSelectList("t.policy") ;
    selectString.addSelectList("t.disseminationpackingalgo_id") ;
    selectString.addSelectList("i.qualifierfile") ;
    selectString.addSelectList("i.validitystop") ;
    selectString.addSelectList("i.qualityinfo") ;
    selectString.addSelectList("q.name") ;
    selectString.addTableList("t_temporarydistribution", "t");
    
    // Add Join
	{
    	dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "u","userid" , dbParam::EQUAL_TO , "t.userid" )) ;
		selectString.addJoinList(SQLDialect::LEFT,"t_users","u", pp) ;
	}
    // Add Join
	{
    	dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "i","filename" , dbParam::EQUAL_TO , "t.filename" )) ;
		pp.addParameter(dbParam(k, "i","fileversion" , dbParam::EQUAL_TO , "t.fileversion" )) ;
		selectString.addJoinList(SQLDialect::LEFT,"t_inventory","i", pp) ;
	}
    // Add Join
	{
    	dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "q","id" , dbParam::EQUAL_TO , "i.qualified" )) ;
		selectString.addJoinList(SQLDialect::LEFT,"t_qualifiedflag","q", pp) ;
	}
    //add Where
	{
    	dbQueryParameters pp(k) ;
	    pp.addParameter( dbParam(k, "t" , "preselection" , dbParam::EQUAL_TO , db::toSQL( k , true ) ) ) ;
	    pp.addParameter( dbParam(k, "t" , "orderid" , dbParam::ISNULL  ) ) ;
	    pp.addParameter( dbParam(k, "t" , "algo" , dbParam::EQUAL_TO , db::toSQL( k , algoid ) ) ) ;
		selectString.addANDWhereParameter(pp) ;
  	}
    //add Or
	{
    	dbQueryParameters pp(k) ;
	    pp.addParameter( dbParam(k, "t" , "preselection" , dbParam::EQUAL_TO , db::toSQL( k , true ) ) ) ;
	    pp.addParameter( dbParam(k, "u" , "allqualified" , dbParam::EQUAL_TO , db::toSQL( k , false )  ) ) ;
	    pp.addParameter( dbParam(k, "t" , "algo" , dbParam::EQUAL_TO , db::toSQL( k , algoid ) ) ) ;
        selectString.addORWhereParameter(pp) ;
  	}
    dbQuery selectQuery(theConnection, selectString, "SelQualValStop") ;
    dbSet s ;
    selectQuery.execForSet(s) ;
    

	vector <TemporaryInfo> toBePromoted ;
    vector <dbPersistent::IdType> toBeDeleted ;
    dbRow row(k) ;    
    DateTime now ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "s.size()" << s.size() 	)
    while ( s.getNextRow(row) ) // iterates on each row
    {
        if (stop_pred()) { return ; }
        try
        {
            string availability;
            row.column( row.columns() - 1  ).column( availability ) ; // PRQA S 3000

            string qualInfoString ;
	        bool isNull = false ;
	        row.column( row.columns() - 2  ,  qualInfoString, &isNull  ) ; // PRQA S 3000, 4400
	    
	        if (isNull)
	        {
		        qualInfoString = "FALSE";
	        }
            bool qualInfo = StringUtils::equalsNoCase(qualInfoString,"TRUE") ; // PRQA S 4412

            if (availability  == "NOT_AVAILABLE")
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NOT_AVAILIABLE, therefore skipping")
		        continue ;
            }

            bool avaliableCheckPassed = false ;

            if (_qualOk)
            {
                if (availability == "UNMARKABLE")
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UNMARKABLE, therefore passing")
                    avaliableCheckPassed = true ;
                }
                else if ((qualInfo == true) && (availability == "VALID_PRODUCT"))
                {
		            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VALID_PRODUCT && qualInfo == true, therefore passing")
                    avaliableCheckPassed = true ;
                }
                else if ((qualInfo == false) && (availability != "NOT_AVAILABLE"))
                {
		            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "qualInfo == false && availability != NOT_AVAILABLE, therefore passing")
                    avaliableCheckPassed = true ;
                }
                else { /* nothing */ }
            }
            else
            {
                if ((availability == "UNMARKABLE") ||  (availability != "NOT_AVAILABLE"))
                {
		            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, availability << " , therefore passing without qualOk ")
                    avaliableCheckPassed = true ;
                }
            }
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "avaliableCheckPassed Passed: " << boolalpha << avaliableCheckPassed )

            bool timeCheckPassed = false ;

            DateTime valStop ;           
	        row.column( row.columns() - 3  ,  valStop  ) ; // PRQA S 3000, 4400

            if (_checkMode == noCheck)
            {
		        timeCheckPassed = true ;
            }
            else if (_checkMode ==  greater)
            {
                long double vsl = valStop.DateTime::operator JD() ; // PRQA S 3050 2
                long double dnw = now.DateTime::operator JD() ;
		        long double delta = fabsl(vsl -  dnw) ;
		        timeCheckPassed = delta > _threshold ;
            }
            else
            {
                long double vsl = valStop.DateTime::operator JD() ; // PRQA S 3050 2
                long double dnw = now.DateTime::operator JD() ;
		        long double delta = fabsl(vsl -  dnw) ;
		        timeCheckPassed = delta < _threshold ; 
            }
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "timecheck Passed: " << boolalpha << timeCheckPassed )

            if (avaliableCheckPassed && timeCheckPassed)
            {                 
                TemporaryInfo ti ;
                bool isNull_in =  false ;

                row.column( 0, ti.id );
                row.column( 1 ,ti.userid );
                row.column( 2 ,ti.mediatype ); // PRQA S 4400 10 
                row.column( 3 ,ti.filename ); 
                row.column( 4 ,ti.fileversion );
                row.column( 5 ,ti.format );
                row.column( 6 ,ti.duedate ); 
                row.column( 7 ,ti.timestamp ); 
                row.column( 8 ,ti.cartid );
                row.column( 9 ,ti.subreq ); 
                row.column(10 ,ti.policy );
                row.column(11 ,ti.dissempkalgoid );
                row.column(12 ,ti.qualifierFile, &isNull_in);
                toBePromoted.push_back(ti) ;
                sendInDistribution(toBePromoted,theConnection) ;
                toBePromoted.clear() ;
            }
            else
	        {
		        string filename ;
		        bool isNull_in = false ;
		        row.column(3, filename, &isNull_in) ; // PRQA S 4400
		        int policy = 0 ;
		        row.column(10 ,policy); // PRQA S 4400
		        ACS_LOG_WARNING("Distribution rejected for file [" << filename << "] of policy " << policy << " - QualityCheck = " << boolalpha << avaliableCheckPassed << " - TimeCheck = " << boolalpha << timeCheckPassed);

                dbPersistent::IdType t_id = 0 ;
                row.column( 0 , t_id) ;
                toBeDeleted.push_back(t_id) ;
                deleteFromTempDist(toBeDeleted, theConnection) ;
                toBeDeleted.clear() ;
            }
        }
        catch(exception &ex) 
        {
            ACS_LOG_NOTIFY_EX( ex );
            appendSummary(TemporaryDistributionIdHasProblem, 1 ) ;
	    }
    }
}


_ACS_END_NAMESPACE

