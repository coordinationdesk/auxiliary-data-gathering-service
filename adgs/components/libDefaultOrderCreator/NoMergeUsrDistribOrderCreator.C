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

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2016/07/22 14:23:28  marfav
	CSGACQ-86
	SQL code review for Oracle 11
	
	Revision 5.7  2016/05/10 21:33:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.6  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.5  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.4  2014/02/06 09:10:45  francesco.avanzi
	passed from dbConv() to db::toSQL()
	
	Revision 5.3  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
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
	
	Revision 2.18  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.17  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.16  2013/05/14 18:50:17  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.15  2013/04/29 16:01:44  marpas
	file type recognition i/f changed
	
	Revision 2.14  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.13  2013/04/08 09:39:06  marpas
	fixing bug on waiting time computation
	
	Revision 2.12  2013/02/26 18:20:56  marpas
	dbSet new I/F adopted
	
	Revision 2.11  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.10  2012/10/26 10:40:24  marpas
	adding triggering user distribution to order parameters clob
	
	Revision 2.9  2012/10/25 13:29:07  marpas
	processing of invalid files allowed if configured
	
	Revision 2.8  2012/10/25 12:56:00  marpas
	debug improved
	
	Revision 2.7  2012/10/25 12:45:22  marpas
	fixed query
	
	Revision 2.6  2012/10/25 08:31:23  marpas
	distributed files query improved
	
	Revision 2.5  2012/10/24 17:22:16  marpas
	new configuration concept
	
	Revision 2.4  2012/10/23 09:52:09  marpas
	query fixed
	
	Revision 2.3  2012/10/23 09:50:22  marpas
	query fixed
	
	Revision 2.2  2012/10/23 09:40:21  marpas
	algo fixed
	
	Revision 2.1  2012/09/18 09:53:27  marpas
	NoMergeUsrDistribOrderCreator class added to the repository
	
	
	
*/


#include <NoMergeUsrDistribOrderCreator.h>
#include <ConfigurationSingleton.h>
#include <XMLOrderParameters.h>
#include <TemporaryOrder.h>
#include <FileGuard.h>
#include <Filterables.h>
#include <dbConnection.h>
#include <dbFileTypeRecognition.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <dbOrder.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConv.h>
#include <ftmpstream.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(NoMergeUsrDistribOrderCreator) 

NoMergeUsrDistribOrderCreator::NoMergeUsrDistribOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &) :
	OrderCreator(type,algo, satId) ,
    _users_ftype(),
    processInvalidFiles_(false) 
{
}

NoMergeUsrDistribOrderCreator::~NoMergeUsrDistribOrderCreator() throw() {}

string NoMergeUsrDistribOrderCreator::checkIfTriggeringDistributed( // PRQA S 4020, 4214
    const TemporaryOrder &tmporder, 
    dbConnection &theConnection) const 
{

    const string &dbKey = theConnection.getKey() ;

	SQLString sqlString(dbKey, SQLString::SELECT) ;
	// DISTINCT and READ ONLY
	sqlString.setDistinct (true);
	sqlString.setReadOnlySelect (true);

    sqlString.addSelectList("d.userid");
    sqlString.addTableList( "t_distributionitems", "d" ) ;

	// JOIN
	{
    	dbQueryParameters pp(dbKey) ;
		pp.setParamOperator ( dbQueryParameters::AND ) ;
		pp.addParameter(dbParam(dbKey, "d", "packageid", dbParam::EQUAL_TO , "q.packageid")) ; //  d.packageid = q.packageid
		pp.addParameter(dbParam(dbKey, "q", "status",    dbParam::EQUAL_TO , db::toSQL(dbKey, "COMPLETED") )) ; //  q.status = 'COMPLETED'
    	sqlString.addJoinList(SQLDialect::INNER, " t_distributionqueue", "q", pp) ;
	}

	// WHERE
	{
		dbQueryParameters pars(dbKey) ;
		pars.setParamOperator ( dbQueryParameters::AND ) ;
		pars.addParameter(dbParam(dbKey, "d", "filename", dbParam::EQUAL_TO,db::toSQL(dbKey, tmporder.fileName()))) ;
		pars.addParameter(dbParam(dbKey, "d", "fileversion", dbParam::EQUAL_TO,db::toSQL(dbKey, tmporder.fileVersion()))) ;
		sqlString.addANDWhereParameter(pars) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "query is: \"" << sqlString.getSQL() ) ;

    dbQuery compQuery(theConnection,sqlString,"CheckIfDistributed") ;
    dbSet compSet ;
    compQuery.execForSet(compSet) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "query returned " << compSet.size() << " records") ;
    size_t rcount = compSet.size() ;
	for (size_t i=0; i < rcount ; ++i) {
		dbRow row(dbKey) ;
		compSet.getNextRow(row) ;
		string user ;
        row.column(0,user) ;
        if (checkFtypeAgainstConf(tmporder.fileType(), user, theConnection)) {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ftype " << tmporder.fileType() << " user: " << user << " match") ;
            return user ;
        }
	}
    return "" ;
}

bool NoMergeUsrDistribOrderCreator::checkFtypeAgainstConf(const std::string &ftype, const std::string &user, dbConnection &theConnection) const  // PRQA S 4020, 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ftype " << ftype << " user: " << user) ;
    std::map<std::string, std::vector<std::string> >::const_iterator vf = _users_ftype.find(user) ;
    if (vf == _users_ftype.end()) {
        return false ;
    }
	dbFileTypeRecognition ftr;
    for(size_t i = 0; i < vf->second.size(); ++i) {
        if (ftype == vf->second[i] || ftr.relationship(ftype, vf->second[i], theConnection)) {
            return true ;
        }
    }
    
    return false ;
}

void NoMergeUsrDistribOrderCreator::appendTmpOrder( // PRQA S 4214
    double maxTime, 
    const TemporaryOrder &order, 
    vector<pair<TemporaryOrder, string> > &orders, 
    dbConnection &theConnection) const
{
	if(order.validityFlag() || processInvalidFiles_ ) { 
		maxTime = getMaxWaitingTime("") ;
		ostringstream msg ;
		try {
			maxTime = getMaxWaitingTime("",order.fileType()) ;
			msg << algo() << ": Using waiting time for filetype \"" << order.fileType() << "\" " << maxTime << " [sec]" ;
		}
		catch (exception &x) {
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
				ACS_LOG_NOTIFY_EX(x) ;
				ACS_LOG_WARNING("The above exception has been ignored: using default") ;
			ACS_CLASS_END_DEBUG
			msg << algo() << ": Using default waiting time for fileType \"" << order.fileType() << "\" " << maxTime << " [sec]" ;
		}
		if( order.isTmpOrderExpired(maxTime)) {
            try {
                string userdistr = checkIfTriggeringDistributed(order, theConnection) ;
                if (not userdistr.empty()) {
					orders.push_back(make_pair(order, userdistr)) ;
					ACS_LOG_INFO(msg.str()) ;
                }
            }
            catch(exception &x) {
                ACS_LOG_NOTIFY_EX(x) ;
                ACS_LOG_WARNING("The temporary order #" << order.id() << " will be ignored until the above error will persist.") ;
				appendSummary(TemporaryOrderIdHasProblem,order.id()) ;
             }
		}
	} else {	

		ACS_LOG_WARNING("The temporary order id " << order.id()
			<< " cannot be processed because it refers to file \"" << order.fileName() << "\"" 
			<< " currently invalid.") ;
		appendSummary(TemporaryOrderIdHasProblem,order.id()) ;
	}
}

void NoMergeUsrDistribOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	// **** SUBSECTION 1 *** configuration stuff
	try {
		loadConf() ; // load configuration
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e) ;
		ACS_LOG_ERROR("giving up because configuration not aligned") ;
		return ;
	}
    const string &dbKey = theConnection.getKey() ;
	// find unique temporary orders with satelliteid, ordertype and algo
	string UniqueFileTypesCommand = TemporaryOrder::getUniqueEntriesSqlCommand(dbKey,type(), algo(), satelliteId()) ;
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(UniqueFileTypesCommand) ;
	dbQuery fTypeQuery(theConnection,sqlString, "SelUniqTmpOrders") ;
	dbSet fTypeSet;
	fTypeQuery.execForSet(fTypeSet) ;
	vector<TemporaryOrder::UniqueEntry> uniqueEntries ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unique file types in temporary orders are " << fTypeSet.size()) ;
    size_t rcount = fTypeSet.size() ;
	for (size_t i=0; i < rcount; ++i) {
		dbRow row(dbKey) ;
		fTypeSet.getNextRow(row) ;
		TemporaryOrder::UniqueEntry entry ;
		entry.loadRow(row) ;
		if (std::find(uniqueEntries.begin(),uniqueEntries.end(), entry) == uniqueEntries.end()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t[" << i << "] " << entry.filetype << " "  << entry.processorName  << " "  << entry.processorVersion << " "  << entry.processorConfiguration)  ;
 			uniqueEntries.push_back(entry) ;
		}
	}

	vector<pair<TemporaryOrder, string> > orders ;
	
	double maxTime = getMaxWaitingTime("") / 86400.0; // PRQA S 4412
	
	// get every needed temporary order
    size_t uesize = uniqueEntries.size() ;
	for (size_t tmpOrder = 0; tmpOrder < uesize; ++tmpOrder) {

        if (stop_pred()) { return ; }

		string sqlCommand = TemporaryOrder::getSqlCommandForFType(dbKey,type(), algo(), uniqueEntries[tmpOrder]) ;
		SQLString sqlString1(dbKey, SQLString::COMMAND) ;
		sqlString1.forceSQLString(sqlCommand) ;
		dbQuery getQuery(theConnection,sqlString1, "SelFtypeTmpOrders") ;
		dbSet set;
		getQuery.execForSet(set) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "createOrders found "<< set.size() << " object for " << type() << " " << algo() << " " << uniqueEntries[tmpOrder].processorName) ;
        size_t rcount1 = set.size() ;
		for(size_t r=0; r<rcount1; ++r)
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
		createOrder(orders[order].first, orders[order].second, theConnection) ;
	}
}

void NoMergeUsrDistribOrderCreator::createOrder(const TemporaryOrder &tempOrder, const string &userdistrib, dbConnection &theConnection) const // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	dbOrder *order= dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", theConnection.getKey())); // PRQA S 3081, 4412
	if(!order) {
		TemporaryOrder::OrderException ex("Cannot allocate a dbOrder") ;
		ACS_THROW(ex) ;
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
	int mission = 0 ;
	bool missionNull = false ;
	mission = tempOrder.mission(&missionNull) ;
	if (!missionNull) {
		order->setMission(mission) ;
    }

    XMLOrderParameters ops ;
    ops.setTriggeringFiles(getTriggeringFiles(tempOrder),userdistrib) ;
	oftmpstream params("/tmp/ordpar_", "xml") ;
    ops.write(params()) ;
    params.close() ; // causes the flush
	order->setParametersUploadFileName(params.path()) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
	    params.setAutoRemove(false) ;
		ostringstream dbg ;
        dbg << "dump\n" ;
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
        dbg << "param file left on disk at: " << params.path() << "\n" ;
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

string NoMergeUsrDistribOrderCreator::getAlgoKeyString() const 
{
    return "NoMergeUserDistributed" ;
}


void NoMergeUsrDistribOrderCreator::loadConf() const // PRQA S 4214
{
    // this cause an exception if not found: leave it here.
	size_t nusers = ConfigurationSingleton::instance()->get().getArraySize(buildKey("user","users")) ; // PRQA S 4412
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "number of users: " << nusers ) ;

    vector<string> users ;
    users.reserve(nusers) ;
    ConfigurationSingleton::instance()->get().getArray(buildKey("user","users"), users) ;

	for (size_t i=0; i<nusers; ++i) {
		ostringstream osuser ;
		osuser << "user[" << i << "]" ;
		string ftypesuffix = ".filetypes.filetype" ;
		size_t nfiletypes = ConfigurationSingleton::instance()->get().getArraySize(buildKey(osuser.str()+ftypesuffix,"users")) ; // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "number of filetypes for user[" << i << "](" << users[i] << "): " << nfiletypes) ; // PRQA S 3000
		vector<string> ftypes ;
        ftypes.reserve(nfiletypes) ;
		ConfigurationSingleton::instance()->get().getArray(buildKey(osuser.str()+ftypesuffix,"users"), ftypes) ; // PRQA S 4412
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
            ostringstream dbg ;
            dbg << "user[" << i << "](" << users[i] << "): \n" ;
            size_t ftsize = ftypes.size() ;
			for (size_t f = 0; f < ftsize; ++f) { 
				dbg << "[" << f << "]: " << ftypes[f] << "\n" ;
			}
            ACS_LOG_DEBUG(SimpleDebugSignature << dbg.str()) ;
		ACS_CLASS_END_DEBUG
		_users_ftype[users[i]] = ftypes ;
	}
	double _maxTime = getMaxWaitingTime("") / 86400.0; // PRQA S 4400, 4412
    try {
        string key ;
	    getConf("ProcessInvalidFiles", key, processInvalidFiles_, "") ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ProcessInvalidFiles:" << boolalpha << processInvalidFiles_) ;
    }
    catch(exception &x) {
        ACS_LOG_WARNING(algo() << ": cannot find ProcessInvalidFiles parameter using default: " << boolalpha << processInvalidFiles_) ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TemporaryOrdersMaxWaitingTime: " << _maxTime << " [days]") ;
}

_ACS_END_NAMESPACE
