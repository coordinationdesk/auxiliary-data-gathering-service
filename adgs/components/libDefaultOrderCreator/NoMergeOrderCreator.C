// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 5.15  2017/10/20 08:34:25  davide.tiriticco
	Added setDupAllowed and removed exception throw from getAlgoKeyString
	
	Revision 5.14  2016/07/22 14:23:28  marfav
	CSGACQ-86
	SQL code review for Oracle 11
	
	Revision 5.13  2016/07/14 14:14:20  marfav
	Alias SQL syntax fixed in JOIN clauses
	
	Revision 5.12  2016/05/10 21:33:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.11  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.10  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.9  2016/01/11 17:43:39  francesco.avanzi
	print enhanced
	
	Revision 5.8  2015/04/21 15:30:52  marfav
	Adding the possibility to expand nomerge by inheritance
	
	Revision 5.7  2015/02/27 10:36:21  marfav
	APF-259 added baseline management
	
	Revision 5.6  2014/05/15 18:10:10  marpas
	exception message fixed
	
	Revision 5.5  2014/02/06 09:13:22  francesco.avanzi
	passed from dbConv() to db::toSQL()
	
	Revision 5.4  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
	Revision 5.3  2013/10/03 13:09:52  marpas
	DUP or invalid errors must not be processed after their removal
	
	Revision 5.2  2013/07/22 16:44:04  marpas
	compilation warnings removed
	
	Revision 5.1  2013/07/22 13:42:06  marpas
	ADC-193 implemented
	coding best practices enforced
	compilation and qa warnings fixed
	
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.26  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.25  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.24  2013/05/31 14:17:04  marfav
	Fixed time convertion in appendTmpOrder when computing max waiting time
	
	Revision 2.23  2013/05/14 18:50:17  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.22  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.21  2013/04/08 09:39:06  marpas
	fixing bug on waiting time computation
	
	Revision 2.20  2013/03/08 17:35:39  marpas
	DUP tmp orders are removed as soon as they are discovered
	
	Revision 2.19  2013/03/08 10:04:28  marpas
	fixing some anomalies and asymmetries with other classes
	
	Revision 2.18  2013/03/07 19:47:16  marpas
	DUP inventory management as in jira ADC-122
	
	Revision 2.17  2013/02/26 18:20:56  marpas
	dbSet new I/F adopted
	
	Revision 2.16  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.15  2012/03/29 13:49:19  marpas
	forcing flush - closure in parameters file
	
	Revision 2.14  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.13  2010/07/09 16:12:05  marpas
	useless messages removed
	
	Revision 2.12  2010/06/24 10:26:41  marpas
	satellite id comaprison fixed (null case)
	
	Revision 2.11  2010/03/26 13:24:19  marpas
	messaging slightly changed: no msg in max waiting time default
	
	Revision 2.10  2010/03/25 14:07:55  marpas
	Allowing to search for TemporaryOrdersMaxWaitingTime in namespace:
	<OrderGeneration><"ordertype"><"Sat"><"NoMerge|Level1|Level2"><"Processor"> if not found:
	<OrderGeneration><"ordertype"><"Sat"><"NoMerge|Level1|Level2"> will be used as before (default)
	
	Revision 2.9  2010/03/08 13:43:36  enrcar
	EC:: Oracle DB handled
	
	Revision 2.8  2009/07/01 16:26:01  marpas
	order mission is set for MERGE and NOMERGE
	
	Revision 2.7  2009/05/08 14:57:17  marpas
	concept of aging policy on temporary introduced - no regression by default aging policy
	
	Revision 2.6  2009/05/07 09:49:06  marpas
	TemporaryOrder::isExpired method is now deprecated. Call isInventoryExpired instead. this is due to libOrderCreator Rev_2_20 to solve the VEN-1198
	
	Revision 2.5  2008/12/01 18:17:21  marpas
	debug message fixed
	
	Revision 2.4  2008/11/27 14:02:17  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.3  2008/11/24 16:42:52  marpas
	triggering files are now inserted into t_ordtriggeringfiles
	
	Revision 2.2  2008/10/07 12:43:47  crivig
	porting to postgres
	
	Revision 2.1  2006/05/31 18:28:52  giucas
	Added MaxWaitingTime handling for NoMerge algo
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/


#include <NoMergeOrderCreator.h>
#include <XMLOrderParameters.h>
#include <TemporaryOrder.h>
#include <FileGuard.h>
#include <Filterables.h>
#include <dbConnection.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbOrder.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConv.h>
#include <ftmpstream.h>

#include <dbSchemaMetaDataProvider.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(NoMergeOrderCreator) 

NoMergeOrderCreator::NoMergeOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &conn, bool dupAllowed) :
	OrderCreator(type,algo, satId) ,
	_agingOnTmpTStamp(false),
    _dupAllowed(dupAllowed),
    _isSticky(),
    _readMaxTime(true)
{
    if (not _dupAllowed) {
        loadAllowedQualified(conn.getKey()) ;
    }
}

NoMergeOrderCreator::~NoMergeOrderCreator() ACS_NOEXCEPT {}

void NoMergeOrderCreator::setDupAllowed(bool v) // PRQA S 4121
{
	_dupAllowed = v ;
}

void NoMergeOrderCreator::setAgingPolicyOnTmpOrder(bool v) // PRQA S 4121
{
	_agingOnTmpTStamp = v ;
}

bool NoMergeOrderCreator::getAgingPolicyOnTmpOrder() const  // PRQA S 4120
{
	return _agingOnTmpTStamp ;
}

void NoMergeOrderCreator::appendTmpOrder(double maxTime, const TemporaryOrder &order, vector<TemporaryOrder> &orders, dbConnection &conn) const // PRQA S 4214
{
    if (not _dupAllowed) { // check for dup and qualified flag
        bool remove = dupOrNotQualified(order) ;
        if (remove) {
            try {
                if (!simulation()) {
                    order.removeFromDb(conn) ;
                }
                ACS_LOG_WARNING("The temporary order id " << order.id()
                    << " has been deleted because it refers to file \"" << order.fileName() << "\""
                    << " currently marked as DUP or not qualified") ;
                if (simulation()) {
                    ACS_LOG_WARNING("above deletion has been simulated") ;
                }
            }
            catch(exception &x) {
                ACS_LOG_ERROR("Cannot remove tmp order id: " << order.id() << " because: "
                    << x.what() // not all the stack
                    << "Processing continue: will retry later") ;
            }
            return ; // no more processing on this one.
        }
    }
	if(order.validityFlag()) {
        ostringstream msg ; 
        msg << "[tmpid: " << order.id() << "] " ;
		maxTime = getMaxWaitingTime("") / 86400.0 ; // PRQA S 4400
		try {
			maxTime = getMaxWaitingTime("",order.processorName()) / 86400.0 ; // PRQA S 4400
			msg << algo() << ": Using waiting time for processor \"" << order.processorName() << "\" " << maxTime << " [sec]" ;
		}
		catch (exception &x) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, x << "This exception has been ignored: using default") ;
			msg << algo() << ": Using default waiting time for processor \"" << order.processorName() << "\" " << maxTime << " [sec]" ;
		}

		bool append = false ;

        if (_agingOnTmpTStamp == false) { // as usual by default argument value
			if( order.isInventoryExpired(maxTime)) { append = true ; }
		}
		else { // new tmp order tstamp aging means _agingOnTmpTStamp == true
			if( order.isTmpOrderExpired(maxTime)) { append = true ; }
		}

        if (append) {
            orders.push_back(order) ;
			ACS_LOG_INFO( msg.str() ) ; // print the message only if it is a candidate
        }

        
	} else {	

		ACS_LOG_WARNING("The temporary order id " << order.id()
			<< " cannot be processed because it refers to file \"" << order.fileName() << "\"" 
			<< " currently invalid.") ;
		appendSummary(TemporaryOrderIdHasProblem,order.id()) ;
	}
}

void NoMergeOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

	// find unique temporary orders with satelliteid, ordertype and algo
	string UniqueFileTypesCommand = TemporaryOrder::getUniqueEntriesSqlCommand(dbKey, type(), algo(), satelliteId()) ;
	SQLString sqlStringUe(dbKey, SQLString::COMMAND) ;
	sqlStringUe.forceSQLString(UniqueFileTypesCommand) ;
	dbQuery fTypeQuery(theConnection,sqlStringUe, "SelUniqTmpOrders") ;
	dbSet fTypeSet;
	fTypeQuery.execForSet(fTypeSet) ;
	vector<TemporaryOrder::UniqueEntry> uniqueEntries ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"unique file types in temporary order are " << fTypeSet.size()) ;
    
    size_t rcount = fTypeSet.size() ;
	for (size_t i=0; i < rcount; ++i) {
		dbRow row(dbKey) ;
		fTypeSet.getNextRow(row) ;
		TemporaryOrder::UniqueEntry entry ;
		entry.loadRow(row) ;
		if (std::find(uniqueEntries.begin(),uniqueEntries.end(), entry) == uniqueEntries.end()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\t[" << i << "] " << entry.filetype << " "  << entry.processorName  << " "  << entry.processorVersion << " "  << entry.processorConfiguration) ;
			uniqueEntries.push_back(entry) ;
		}
	}

	vector<TemporaryOrder> orders ;
	
	double maxTime = 0;
    if (_readMaxTime)
    {
        // This can be disabled by derived classes who do not need to use the max time 
        maxTime = getMaxWaitingTime("") / 86400.0; // PRQA S 4412
    }
	
	// get every needed temporary order
    size_t uesize = uniqueEntries.size() ;
	for (size_t tmpOrder = 0; tmpOrder < uesize; ++tmpOrder) {

        if (stop_pred()) { return ; }

		string sqlCommand = TemporaryOrder::getSqlCommandForFType(dbKey, type(), algo(), uniqueEntries[tmpOrder]) ;
		SQLString sqlString(dbKey, SQLString::COMMAND) ;
		sqlString.forceSQLString(sqlCommand) ;
		dbQuery getQuery(theConnection,sqlString, "SelFtypeTmpOrders") ;
		dbSet set;
		getQuery.execForSet(set) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"createOrders found "<< set.size() << " object for " << type() << " " << algo() << " " << uniqueEntries[tmpOrder].processorName) ;
        
        size_t rcount2 = set.size() ;
		for(size_t r=0; r<rcount2; ++r)
		{
			TemporaryOrder order ;
			dbRow row(dbKey) ;
			set.getNextRow(row);
			order.loadRow(row) ;
            appendTmpOrder(maxTime, order, orders, theConnection) ;
		}	
	}

    size_t osize = orders.size() ;
	for (size_t order=0; order < osize; ++order) {
        if (stop_pred()) { return ; }
                const TemporaryOrder& tempOrder = orders[order];
        	try {
			createOrder(tempOrder, theConnection) ;
		} catch (exception& e) {
			ACS_LOG_WARNING("The processing of temporary order id " << tempOrder.id()
				<< " that refers to file \"" << tempOrder.fileName() << "\"" 
				<< " exited with following error:\n" << e.what() << " Processing continues.") ;
			appendSummary(TemporaryOrderIdHasProblem, tempOrder.id());			
		} catch (...) {
			ACS_LOG_WARNING("The processing of temporary order id " << tempOrder.id()
				<< " that refers to file \"" << tempOrder.fileName() << "\"" 
				<< " is ended unexpectedly. Processing continues.") ;
			appendSummary(TemporaryOrderIdHasProblem, tempOrder.id()) ;
		}
	}
}

void NoMergeOrderCreator::createOrder(const TemporaryOrder &tempOrder, dbConnection &theConnection) const // PRQA S 4020
{
	dbOrder *order= dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", theConnection.getKey())); // PRQA S 3081, 4412
	if(!order) {
		ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbOrder")) ; // PRQA S 3081
	}
	
	// t_orderdQueue description
	//
// // 	orderid 				SERIAL not null,
// // 	creationdate			DATETIME YEAR TO FRACTION not null,
// // 	priority 				SMALLINT not null,
// // 	status 					NCHAR(32) not null,
// // 	processorname 			NCHAR(32) not null,
// // 	processorversion		NCHAR(8) not null,
// // 	processorconfiguration	NCHAR(8),
// // 	troubleshooting			boolean not null,
// // 	starttime 				DATETIME YEAR TO FRACTION not null,
// // 	stoptime 				DATETIME YEAR TO FRACTION not null,
// // 	jobresponsible 			NCHAR(16),
// // 	productionstarttime 	DATETIME YEAR TO FRACTION,
// // 	productionstoptime 		DATETIME YEAR TO FRACTION,
// // 	processingtimewindow 	SMALLINT not null,
// // 	processorid 			NCHAR(16),
// // 	ordertype 				NCHAR(8) not null,
// // 	externalorderid 		NCHAR(32)
// //   satelliteid 			SMALLINT 

	dbPersistentScopeGuard theOrderScopeGuard(order) ;
	DateTime	dt ;
	order->setCreationDate(dt) ;
	order->setPriority(tempOrder.priority()) ;
	order->setStatus("ELIGIBLE") ;
	order->setProcessorName(tempOrder.processorName()) ; 
	order->setProcessorVersion(tempOrder.processorVersion()) ;
	order->setProcessorConfiguration(tempOrder.processorConfiguration()) ;
	order->setTroubleshooting(tempOrder.troubleShooting()) ;
	order->setStartTime(tempOrder.start()) ;
	order->setStopTime(tempOrder.stop()) ;
	order->setProcessingTimeWindow(tempOrder.processingTimeWindow()) ;
	order->setOrderType(tempOrder.orderType()) ;
	// set the satellite id inside the order if not null
	if (satelliteId() != 0 ) {
		order->setSatelliteId(satelliteId()) ;
    }
	int mission = 0;
	bool missionNull = false ;
	mission = tempOrder.mission(&missionNull) ;
	if (!missionNull) {
		order->setMission(mission) ;
    }
    if (_isSticky) { 
        string jr =getJobResponsible(tempOrder.fileName(), tempOrder.fileVersion(), theConnection) ;
        if (not jr.empty()) {
            order->setJobResponsible(jr) ;
        }
    }
	
    XMLOrderParameters ops ;
    ops.setTriggeringFiles(getTriggeringFiles(tempOrder)) ;
	oftmpstream params("/tmp/ordpar_", "xml") ;

    static const string baselinetable ("t_baselines_x_processors");
    // If the processor to baseline relation is allowed, do manage it
    if (dbSchemaMetaDataProvider::existTable (baselinetable, theConnection))
    {
        XMLOstream::Tag tbl (ops.xml() , "Baseline" , getBaseline (order->getProcessorName(), order->getProcessorVersion(), theConnection ), "", true);
    }

    ops.write(params()) ;
    params.close() ; // causes the flush
	order->setParametersUploadFileName(params.path()) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        ostringstream dbg ;
	    params.setAutoRemove(false) ;
		dbg << "dump \n" ;
		ostringstream os ;
		order->getCreationDate().utcs(os) ;
		dbg << "CreationDate " << os.str() << "\n" ;
		dbg << "Priority " << order->getPriority() << "\n" ;
		dbg << "Status " << order->getStatus() << "\n" ;
		dbg << "Troubleshooting " << order->getTroubleshooting() << "\n" ;
		ostringstream os1 ;
		order->getStartTime().utcs(os1) ;
		dbg << "StartTime " <<  os1.str() << "\n" ;
		ostringstream os2 ;
		order->getStopTime().utcs(os2) ;
		dbg << "StopTime " <<  os2.str() << "\n" ;
		dbg << "ProcessingTimeWindow " << order->getProcessingTimeWindow() << "\n" ;
		dbg << "ProcessorName " << order->getProcessorName() << "\n" ; 
		dbg << "ProcessorVersion " << order->getProcessorVersion() << "\n" ;
		dbg << "ProcessorConfiguration " << order->getProcessorConfiguration() << "\n" ;
		dbg << "OrderType " << order->getOrderType() << "\n" ;
		dbg << "JobResponsible " << order->getJobResponsible() << "\n" ;
		ostringstream os3 ;
		{
			bool set = false ;
			order->getSatelliteId(&set) ;
			if (set) {
				os3 << order->getSatelliteId() ;
            }
			else {
				os3 << "<null>" ;
            }
		}
		dbg << "SatelliteId " << os3.str() << "\n" ;
        dbg << "param file left on disk at: " << params.path() ;
       ACS_LOG_DEBUG(SimpleDebugSignature << dbg.str()) ;
	ACS_CLASS_END_DEBUG

	
	db::Transaction transaction(theConnection) ;
	transaction.begin_work("") ; 
	
	ACS_LOG_INFO("Emitting order " << order->getOrderType() 
		<< " for processor " << order->getProcessorName() << " " << order->getProcessorVersion() 
		<< " conf " << order->getProcessorConfiguration()) ;
	ostringstream ostart ;
    ostringstream ostop ;
	order->getStartTime().utcs(ostart) ;
	order->getStopTime().utcs(ostop) ;
	ACS_LOG_INFO("               " << "Processing window: " << ostart.str() << " " << ostop.str()) ;

	try {
	
		if (!simulation()) {
			// save the order (remember is inside a transaction) 
			order->save(theConnection) ;
			insertTriggeringFile(tempOrder, order->getOrderId(), theConnection) ;
		}
		
		if (!simulation()) {
			// remove the temporay orders
            const string &dbKey = theConnection.getKey() ;

			SQLString sqlString(dbKey, SQLString::DELETE) ;
			string table = "t_temporaryOrders" ;
            sqlString.addTableList( table ) ;
			{
                dbQueryParameters pp(dbKey) ; 
				pp.addParameter(dbParam(dbKey,table,"id", dbParam::EQUAL_TO, db::toSQL(dbKey, tempOrder.id()))) ; // PRQA S 3050, 3081
                sqlString.addANDWhereParameter(pp) ;
    		}

			dbQuery deleteQuery(theConnection,sqlString, "DeleteTmpOrder") ;
			deleteQuery.execForStatus() ;
		}
		
		transaction.commit() ; 
		ACS_LOG_INFO("The order id: " << order->getOrderId() << " has just been succesfully emitted.") ;
		appendSummary(OrderIdSuccesfullyCreated,order->getOrderId()) ;
	}
	catch (...) {
		transaction.roll_back() ;
		throw ;
	}
}

string NoMergeOrderCreator::getAlgoKeyString() const // PRQA S 4020
{
    if (StringUtils::equalsNoCase(algo(), "NOMERGE")) {
        return "NoMerge" ;
    }
    if (StringUtils::equalsNoCase(algo(), "NOMERGENODUP")) {
        return "NoMergeNoDup" ;
    }
    if (StringUtils::equalsNoCase(algo(), "NOMERGESTICKY")) {
        return "NoMergeSticky" ;
    }
    return "" ;
}

void NoMergeOrderCreator::loadConf() const // PRQA S 4214
{
    double maxTime = 0 ;
	try {
        maxTime = getMaxWaitingTime("") / 86400.0; // PRQA S 4400, 4412
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"TemporaryOrdersMaxWaitingTime: " << maxTime << " [days]") ;
}


string NoMergeOrderCreator::getBaseline (const string& pname, const string& pvers, dbConnection& conn) const
{
    string dbKey = conn.getKey();
    SQLString theSQL(dbKey, SQLString::SELECT );
    theSQL.addSelectList("B.baseline" );
    theSQL.addTableList("t_processors", "P");

    // JOIN on t_baselines_x_processors
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "X","processor_id" , dbParam::EQUAL_TO , "P.id_processor" )) ;
        theSQL.addJoinList(SQLDialect::INNER,"t_baselines_x_processors", "X",pp) ;
    }

    // JOIN on t_baselines
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "B","id_baseline" , dbParam::EQUAL_TO , "X.baseline_id" )) ;
        theSQL.addJoinList(SQLDialect::INNER,"t_baselines" , "B",pp) ;
    }


    // WHERE clauses
    {
        dbQueryParameters pp(dbKey);
        pp.addParameter( dbParam(dbKey, "P" ,"processorname", dbParam::EQUAL_TO, db::toSQL(conn, pname) ));
        theSQL.addANDWhereParameter( pp );
    }

    {
        dbQueryParameters pp(dbKey);
        pp.addParameter( dbParam(dbKey, "P" ,"processorversion", dbParam::EQUAL_TO, db::toSQL(conn, pvers) ));
        theSQL.addANDWhereParameter( pp );
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<theSQL.getSQL());

    dbQuery query ( conn, theSQL , "GET Baseline");

    dbSet mySet;
    query.execForSet (mySet);

    if (mySet.size() == 0)
    {   
        ostringstream os;
        os << "Baseline not defined for the processor [" << pname << ", " << pvers << "]";
        ACS_THROW (exIllegalValueException (os.str()));
    }

    if (mySet.size() > 1)
    {
        ostringstream os;
        os << "Too many baslines defined for the processor [" << pname << ", " << pvers << "]";
        ACS_THROW (exIllegalValueException (os.str()))
    }

    //Extract from the result the baseline name
    dbRow row(dbKey);
    mySet.getNextRow(row);
    string baseline;
    row.column(0, baseline);    
    return baseline;
}

string NoMergeOrderCreator::getJobResponsible(const string &fname, const string &fversion, dbConnection& conn) const 
{
    const string &dbKey = conn.getKey();
    SQLString theSQL(dbKey, SQLString::SELECT );
    theSQL.addSelectList("comment" );
    theSQL.addTableList("t_inventorycomments", "C");

    // WHERE clauses
    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "C","author" , dbParam::EQUAL_TO , db::toSQL(dbKey, "jobresponsible") )) ;
        theSQL.addANDWhereParameter(pp) ;
    }

    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "C","filename" , dbParam::EQUAL_TO , db::toSQL(dbKey, fname) )) ;
        theSQL.addANDWhereParameter(pp) ;
    }

    {
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "C","fileversion" , dbParam::EQUAL_TO , db::toSQL(dbKey, fversion) )) ;
        theSQL.addANDWhereParameter(pp) ;
    }
    
    theSQL.addOrderByList("date", SQLString::DESC);
    theSQL.setLimit(1) ;


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<theSQL.getSQL());

    dbQuery query ( conn, theSQL , "Inv GET JobResp");

    dbSet mySet;
    query.execForSet (mySet);

    if (mySet.size() == 0) { return "" ; }


    //Extract from the result the job responsible name
    dbRow row(dbKey);
    mySet.getNextRow(row);
    string jr;
    row.column(0, jr);    
    return jr;
}


_ACS_END_NAMESPACE
